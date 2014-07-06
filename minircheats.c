#include "minir.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "libretro.h"

//http://msdn.microsoft.com/en-us/library/vstudio/tcxf1dw6.aspx says %zX is not supported
//let's define it to whatever they do support.
#ifdef _WIN32
#define z "I"
#else
#define z "z"
#endif

enum cheat_compto { cht_prev, cht_cur, cht_curptr };
enum cheat_dattype { cht_unsign, cht_sign, cht_hex };
struct minircheatdetail;
static void details_free(struct minircheatdetail * this);
struct minircheats_impl {
	struct minircheats i;
	
	//struct libretro * core;
	struct window * parent;
	
	struct minircheats_model * model;
	
	struct window * wndsrch;
	struct widget_listbox * wndsrch_listbox;
	struct widget_label * wndsrch_nummatch;
	struct widget_radio * wndsrch_comptype;
	struct widget_radio * wndsrch_compto_select;
	struct widget_radio * wndsrch_compto_select_prev;
	struct widget_textbox * wndsrch_compto_entry;
	struct widget_label * wndsrch_compto_label;
	struct widget_radio * wndsrch_dattype;
	struct widget_radio * wndsrch_datendian;
	struct widget_radio * wndsrch_datsize;
	
	struct window * wndlist;
	struct widget_listbox * wndlist_listbox;
	
	struct window * wndwatch;
	
	//This is one random cheat detail window; it's used to destroy them all when changing the core or destroying this object.
	//The rest are available as a linked list. The list is not sorted; the order in which things are
	// destroyed isn't relevant, and that's all it's used for.
	struct minircheatdetail * details;
	
	bool enabled :1;
	bool prev_enabled :1;
	
	unsigned int datsize :3;
	enum cheat_dattype dattype :2;
	
	bool hassearched :1;
	
	char celltmp[32];
};

//out length must be at least 16 bytes; return value is 'out'
static const char * encodeval(enum cheat_dattype dattype, unsigned int datsize, uint32_t val, char * out)
{
	if (dattype==cht_hex)    sprintf(out, "%.*X", datsize*2, val);
	if (dattype==cht_sign)   sprintf(out, "%i", (int32_t)val);
	if (dattype==cht_unsign) sprintf(out, "%u", val);
	return out;
}

static bool decodeval(enum cheat_dattype dattype, const char * str, uint32_t * val)
{
	const char * out;
	if (dattype==cht_hex) *val=strtoul(str, (char**)&out, 16);
	if (dattype==cht_sign) *val=strtol(str, (char**)&out, 10);
	if (dattype==cht_unsign) *val=strtoul(str, (char**)&out, 10);
	return (str!=out && !*out && str[1]!='x' && str[1]!='X');
}

static void search_update(struct minircheats_impl * this);
static void search_update_compto_prev(struct minircheats_impl * this);
static void list_update(struct minircheats_impl * this);

static void set_parent(struct minircheats * this_, struct window * parent)
{
	struct minircheats_impl * this=(struct minircheats_impl*)this_;
	this->parent=parent;
}

static void set_core(struct minircheats * this_, struct libretro * core, size_t prev_limit)
{
	struct minircheats_impl * this=(struct minircheats_impl*)this_;
	while (this->details) details_free(this->details);
	if (core)
	{
		unsigned int nummem;
		const struct retro_memory_descriptor * memory;
		memory=core->get_memory_info(core, &nummem);
		if (memory)
		{
			this->model->set_memory(this->model, memory, nummem);
		}
		else
		{
			void* wram;
			size_t wramlen;
			core->get_memory(core, libretromem_wram, &wramlen, &wram);
			if (wram)
			{
				struct retro_memory_descriptor wramdesc={ .ptr=wram, .len=wramlen };
				this->model->set_memory(this->model, &wramdesc, 1);
			}
			else
			{
				this->model->set_memory(this->model, NULL, 0);
			}
		}
	}
	else
	{
		this->model->set_memory(this->model, NULL, 0);
	}
	this->prev_enabled=(this->model->prev_get_size(this->model) <= prev_limit);
	this->model->prev_set_enabled(this->model, this->prev_enabled);
	search_update_compto_prev(this);
	search_update(this);
	list_update(this);
}




struct minircheatdetail {
	struct minircheats_impl * parent;
	
	struct window * wndw;
	struct widget_textbox * addr;
	struct widget_textbox * newval;
	struct widget_textbox * desc;
	
	unsigned int datsize :3;
	enum cheat_dattype dattype :2;
	//int padding :3;
	//char padding[7];
	
	char orgaddr[32];
	
	//A linked list, so they can be closed.
	struct minircheatdetail * prev;
	struct minircheatdetail * next;
};

static void details_ok(struct widget_button * subject, void* userdata)
{
	struct minircheatdetail * this=(struct minircheatdetail*)userdata;
	uint32_t val;
	if (!decodeval(this->dattype, this->newval->get_text(this->newval), &val))
	{
		this->newval->set_invalid(this->newval, true);
		return;
	}
	const char * orgaddr=this->orgaddr;
	struct cheat newcheat = {
		.addr=(char*)this->addr->get_text(this->addr),
		.datsize=this->datsize,
		.val=val,
		.issigned=(this->dattype==cht_sign),
		.changetype=cht_const,//TODO: make this variable
		.enabled=true,
		.desc=this->desc->get_text(this->desc)
		};
	if (!this->parent->model->cheat_set(this->parent->model, -1, &newcheat))
	{
		this->addr->set_invalid(this->addr, true);
		return;
	}
	int cheatid_org=this->parent->model->cheat_find_for_addr(this->parent->model, this->datsize, orgaddr);
	int cheatid_new=this->parent->model->cheat_find_for_addr(this->parent->model, this->datsize, this->addr->get_text(this->addr));
	if (cheatid_org<0) cheatid_org=this->parent->model->cheat_get_count(this->parent->model);
	if (cheatid_new>=0 && cheatid_org!=cheatid_new)
	{
		//TODO: There is already a cheat set for that address. Do you wish to replace it? (Yes/No)
		this->parent->model->cheat_remove(this->parent->model, cheatid_new);
	}
	this->parent->model->cheat_set(this->parent->model, cheatid_org, &newcheat);
	list_update(this->parent);
	details_free(this);
}

static void details_cancel(struct widget_button * subject, void* userdata)
{
	struct minircheatdetail * this=(struct minircheatdetail*)userdata;
	details_free(this);
}

static bool details_onclose(struct window * subject, void* userdata)
{
	struct minircheatdetail * this=(struct minircheatdetail*)userdata;
	details_free(this);
	return true;
}

static void details_free(struct minircheatdetail * this)
{
	if (this->prev) this->prev->next=this->next;
	else this->parent->details=this->next;
	if (this->next) this->next->prev=this->prev;
	this->wndw->free(this->wndw);
	free(this);
}

static void details_create(struct minircheats_impl * parent, struct window * parentwndw, const struct cheat * thecheat, bool hex)
{
	struct minircheatdetail * this=malloc(sizeof(struct minircheatdetail));
	this->parent=parent;
	this->dattype=hex ? cht_hex : thecheat->issigned ? cht_sign : cht_unsign;
	this->datsize=thecheat->datsize;
	const char * addr=thecheat->addr;
	if (!addr) addr="";
	strcpy(this->orgaddr, addr);
	
	struct widget_textbox * curvalbox;
	struct widget_button * ok;
	struct widget_button * cancel;
	this->wndw=window_create(
		widget_create_layout_vert(
			widget_create_grid(2, 4, false,
				widget_create_label("Address"), this->addr=widget_create_textbox(),
				widget_create_label("Current Value"), curvalbox=widget_create_textbox(),
				widget_create_label("New Value"), this->newval=widget_create_textbox(),
				//TODO: size and type, change mode, etc
				widget_create_label("Description"), this->desc=widget_create_textbox(),
			),
			widget_create_layout_horz(
				widget_create_padding_horz(),
				ok=widget_create_button("OK"),
				cancel=widget_create_button("Cancel"),
				NULL),
			NULL)
		);
	
	this->wndw->set_is_dialog(this->wndw);
	this->wndw->set_parent(this->wndw, parentwndw);
	this->wndw->set_title(this->wndw, "Cheat Details");
	this->wndw->set_onclose(this->wndw, details_onclose, this);
	
	this->addr->set_text(this->addr, addr);
	this->addr->set_width(this->addr, this->parent->model->cheat_get_max_code_len(this->parent->model));
	this->addr->set_length(this->addr, 31);
	
	char valstr[16];
	encodeval(this->dattype, this->datsize, thecheat->val, valstr);
	curvalbox->set_text(curvalbox, valstr);
	this->newval->set_text(this->newval, valstr);//default to keep at current value
	curvalbox->set_enabled(curvalbox, false);
	
	ok->set_onclick(ok, details_ok, this);
	cancel->set_onclick(cancel, details_cancel, this);
	
	if (*addr) this->newval->focus(this->newval);
	else this->addr->focus(this->addr);
	
	this->wndw->set_visible(this->wndw, true);
	this->wndw->focus(this->wndw);
	
	this->next=this->parent->details;
	this->prev=NULL;
	if (this->next) this->next->prev=this;
	this->parent->details=this;
}




static const char * search_get_cell(struct widget_listbox * subject, size_t row, int column, void* userdata);

static void search_update(struct minircheats_impl * this)
{
	if (this->wndsrch_listbox)
	{
		size_t num_rows=this->model->search_get_num_rows(this->model);
		
		this->wndsrch_listbox->set_num_rows(this->wndsrch_listbox, num_rows);
		
		if (this->hassearched)
		{
			char label[64];
			sprintf(label, "%"z"u match%s", num_rows, num_rows==1 ? "" : "es");
			this->wndsrch_nummatch->set_text(this->wndsrch_nummatch, label);
		}
		else this->wndsrch_nummatch->set_text(this->wndsrch_nummatch, "");
	}
}

static void search_set_datsize(struct widget_radio * subject, unsigned int state, void* userdata)
{
	struct minircheats_impl * this=(struct minircheats_impl*)userdata;
	this->datsize=state+1;
	this->model->search_set_datsize(this->model, state+1);
	search_update(this);
}

static void search_set_dattype(struct widget_radio * subject, unsigned int state, void* userdata)
{
	struct minircheats_impl * this=(struct minircheats_impl*)userdata;
	this->dattype=state;
	this->model->search_set_signed(this->model, (state==cht_sign));
	search_update(this);
}

static void search_set_compto_select(struct widget_radio * subject, unsigned int state, void* userdata)
{
	struct minircheats_impl * this=(struct minircheats_impl*)userdata;
	this->wndsrch_compto_entry->set_enabled(this->wndsrch_compto_entry, (state!=cht_prev));
	this->wndsrch_compto_label->set_enabled(this->wndsrch_compto_label, (state!=cht_prev));
}

static void search_split(unsigned int id, void* userdata)
{
	struct minircheats_impl * this=(struct minircheats_impl*)userdata;
	this->model->thread_do_work(this->model, id);
}

static void search_dosearch(struct widget_button * subject, void* userdata)
{
	struct minircheats_impl * this=(struct minircheats_impl*)userdata;
	uint32_t compto_val;
	
	enum cheat_compto comptowhat=(this->wndsrch_compto_select->get_state(this->wndsrch_compto_select));
	bool comptoprev=(comptowhat==cht_prev);
	if (!comptoprev)
	{
		const char * compto_str=(this->wndsrch_compto_entry->get_text(this->wndsrch_compto_entry));
		if (comptowhat==cht_curptr)
		{
			this->model->cheat_read(this->model, compto_str, this->datsize, &compto_val);
		}
		else
		{
			if (!decodeval(this->dattype, compto_str, &compto_val))
			{
				this->wndsrch_compto_entry->set_invalid(this->wndsrch_compto_entry, true);
				return;
			}
		}
	}
	
	this->model->search_do_search(this->model, this->wndsrch_comptype->get_state(this->wndsrch_comptype), comptoprev, compto_val);
	thread_split(this->model->thread_get_count(this->model), search_split, this);
	this->model->thread_finish_work(this->model);
	
	this->hassearched=true;
	search_update(this);
}

static void search_reset(struct widget_button * subject, void* userdata)
{
	struct minircheats_impl * this=(struct minircheats_impl*)userdata;
	this->model->search_reset(this->model);
	this->hassearched=false;
	search_update(this);
}

static void search_add_cheat(struct minircheats_impl * this, int row)
{
	if (row>=0)
	{
		char addr[32];
		
		struct cheat thecheat = {
			.addr=addr,
			.changetype=cht_const,
			.datsize=this->datsize,
			.issigned=(this->dattype==cht_sign),
			.enabled=true
		};
		this->model->search_get_row(this->model, row, thecheat.addr, &thecheat.val, NULL);
		
		details_create(this, this->wndsrch, &thecheat, (this->dattype==cht_hex));
	}
	else
	{
		struct cheat thecheat = {
			.changetype=cht_const,
			.datsize=this->datsize,
			.issigned=(this->dattype==cht_sign),
			.enabled=true
		};
		details_create(this, this->wndsrch, &thecheat, (this->dattype==cht_hex));
	}
}

static void search_add_cheat_listbox(struct widget_listbox * subject, size_t row, void* userdata)
{
	struct minircheats_impl * this=(struct minircheats_impl*)userdata;
	search_add_cheat(this, row);
}

static void search_add_cheat_button(struct widget_button * subject, void* userdata)
{
	struct minircheats_impl * this=(struct minircheats_impl*)userdata;
	search_add_cheat(this, this->wndsrch_listbox->get_active_row(this->wndsrch_listbox));
}

static void search_update_compto_prev(struct minircheats_impl * this)
{
	if (this->wndsrch)
	{
		this->wndsrch_compto_select_prev->set_enabled(this->wndsrch_compto_select_prev, this->prev_enabled);
		if (!this->prev_enabled && this->wndsrch_compto_select->get_state(this->wndsrch_compto_select)==cht_prev)
		{
			this->wndsrch_compto_select->set_state(this->wndsrch_compto_select, cht_cur);
		}
	}
}

static void show_search(struct minircheats * this_)
{
	struct minircheats_impl * this=(struct minircheats_impl*)this_;
	
	if (!this->wndsrch)
	{
		struct widget_button * dosearch;
		struct widget_button * addcheat;
		struct widget_button * reset;
		
		struct widget_radio * compto_select[3];
		
		this->wndsrch=window_create(
			widget_create_layout_vert(
				widget_create_layout_horz(
					this->wndsrch_listbox=widget_create_listbox("Address", "Curr. Value", "Prev. Value", NULL),
					widget_create_layout_vert(
						dosearch=widget_create_button("Search"),
						addcheat=widget_create_button("Add Cheat..."),
						reset=widget_create_button("Reset"),
						this->wndsrch_nummatch=widget_create_label(""),
						widget_create_padding_vert(),
						widget_create_button("Watch"),
						widget_create_button("Clear Watches"),
						widget_create_button("Load Watches"),
						widget_create_button("Save Watches"),
						NULL),
					NULL),
				widget_create_layout_horz(
						widget_create_frame("Comparison Type",
								widget_create_radio_group(&this->wndsrch_comptype, true,
									"< (Less Than)",              "> (Greater Than)",
									"<= (Less Than or Equal To)", ">= (Greater Than or Equal To)",
									"= (Equal To)",               "!= (Not Equal To)",
									NULL)
							),
					widget_create_layout_vert(
						widget_create_frame("Compare To",
							//widget_create_radio_group(&this->wndsrch_compto_select, true, "Previous Value", "Entered Value", "Entered Address", NULL)
							widget_create_layout_vert(
								compto_select[0]=widget_create_radio("Previous Value"),
								compto_select[1]=widget_create_radio("Entered Value"),
								compto_select[2]=widget_create_radio("Entered Address"),
							NULL)
							),
						widget_create_frame("Data Type",
							widget_create_radio_group(&this->wndsrch_dattype, true, "Unsigned (>= 0)", "Signed (+/-)", "Hexadecimal", NULL)
							),
						NULL),
					widget_create_layout_vert(
						widget_create_frame("Data Size",
							widget_create_radio_group(&this->wndsrch_datsize, true, "1 byte", "2 bytes", "3 bytes", "4 bytes", NULL)
							),
						widget_create_padding_vert(),
						NULL),
					NULL),
				widget_create_layout_horz(
					this->wndsrch_compto_label=widget_create_label("Enter a Value: "),
					this->wndsrch_compto_entry=widget_create_textbox(),
					widget_create_button("OK"),
					widget_create_button("Cancel"),
					NULL),
				NULL)
			);
		
		this->wndsrch->set_is_dialog(this->wndsrch);
		this->wndsrch->set_parent(this->wndsrch, this->parent);
		this->wndsrch->set_title(this->wndsrch, "Cheat Search");
		
		compto_select[0]->group(compto_select[0], 3, compto_select);
		this->wndsrch_compto_select=compto_select[0];
		this->wndsrch_compto_select_prev=compto_select[cht_prev];
		search_update_compto_prev(this);
		
		this->wndsrch_listbox->set_contents(this->wndsrch_listbox, search_get_cell, NULL, this);
		const unsigned int tmp[]={15, 15, 15};
		//const unsigned int tmp[]={1,2,2};
		this->wndsrch_listbox->set_size(this->wndsrch_listbox, 16, tmp);
		this->wndsrch_listbox->set_onactivate(this->wndsrch_listbox, search_add_cheat_listbox, this);
		
		dosearch->set_onclick(dosearch, search_dosearch, this);
		reset->set_onclick(reset, search_reset, this);
		
		this->wndsrch_compto_select->set_onclick(this->wndsrch_compto_select, search_set_compto_select, this);
		search_set_compto_select(NULL, 0, this);
		
		this->wndsrch_datsize->set_onclick(this->wndsrch_datsize, search_set_datsize, this);
		search_set_datsize(NULL, 0, this);
		
		this->wndsrch_dattype->set_onclick(this->wndsrch_dattype, search_set_dattype, this);
		search_set_dattype(NULL, 0, this);
		
		addcheat->set_onclick(addcheat, search_add_cheat_button, this);
		
		this->wndsrch_nummatch->set_alignment(this->wndsrch_nummatch, 0);
		this->wndsrch_nummatch->set_ellipsize(this->wndsrch_nummatch, true);
	}
	
	search_update(this);
	
	this->wndsrch->set_visible(this->wndsrch, true);
	this->wndsrch->focus(this->wndsrch);
}

static const char * search_get_cell(struct widget_listbox * subject, size_t row, int column, void* userdata)
{
	struct minircheats_impl * this=(struct minircheats_impl*)userdata;
	
	if (column==0)
	{
		this->model->search_get_row(this->model, row, this->celltmp, NULL, NULL);
	}
	else
	{
		uint32_t val;
		if (column==1) this->model->search_get_row(this->model, row, NULL, &val, NULL);
		if (column==2)
		{
			if (this->prev_enabled) this->model->search_get_row(this->model, row, NULL, NULL, &val);
			else return "---";
		}
		encodeval(this->dattype, this->datsize, val, this->celltmp);
	}
	return this->celltmp;
}





static const char * list_get_cell(struct widget_listbox * subject, size_t row, int column, void* userdata);

static void list_update(struct minircheats_impl * this)
{
	if (this->wndlist_listbox)
	{
		this->wndlist_listbox->set_num_rows(this->wndlist_listbox, this->model->cheat_get_count(this->model));
	}
}

static const char * list_get_cell(struct widget_listbox * subject, size_t row, int column, void* userdata)
{
	struct minircheats_impl * this=(struct minircheats_impl*)userdata;
	struct cheat thecheat;
	thecheat.addr=this->celltmp;
	this->model->cheat_get(this->model, row, &thecheat);
	const char changechar[]="\0+-=";
	switch (column)
	{
		case 0: break;
		case 1: sprintf(this->celltmp, "%.*X%s%c", thecheat.datsize*2, thecheat.val, thecheat.issigned?"S":"", changechar[thecheat.changetype]); break;
		case 2: return thecheat.desc;
	}
	return this->celltmp;
}

static void list_add_cheat(struct widget_button * subject, void * userdata)
{
	
}

static void show_list(struct minircheats * this_)
{
	struct minircheats_impl * this=(struct minircheats_impl*)this_;
	if (!this->wndlist)
	{
		struct widget_button * add;
		this->wndlist=window_create(
			widget_create_layout_vert(
				widget_create_layout_horz(
					this->wndlist_listbox=widget_create_listbox("Address", "Value", "Description", NULL),
					widget_create_layout_vert(
						add=widget_create_button("Add"),
						widget_create_button("Delete"),
						widget_create_button("Update"),
						widget_create_button("Clear"),
						widget_create_button("Sort"),
						widget_create_padding_vert(),
						NULL),
					NULL),
					
					widget_create_grid_v(4, 3, false, false,
						1,1, widget_create_label("Cheat Code"),
						3,1, widget_create_textbox(),
						
						1,1, widget_create_label("Cheat Description"),
						3,1, widget_create_textbox(),
						
						1,1, widget_create_label("Cheat Address (hex)"),
						1,1, widget_create_textbox(),
						1,1, widget_create_label("New Value"),
						1,1, widget_create_textbox()
					),
				NULL)
			);
		
		add->set_onclick(add, list_add_cheat, this);
		
		this->wndlist->set_is_dialog(this->wndlist);
		this->wndlist->set_parent(this->wndlist, this->parent);
		this->wndlist->set_title(this->wndlist, "Cheat Entry and Editor");
		
		this->wndlist_listbox->set_contents(this->wndlist_listbox, list_get_cell, NULL, this);
		//value width is max length of 4294967295 and 0xFFFFFFFF
		//description width is just something random
		const unsigned int tmp[]={this->model->cheat_get_max_code_len(this->model), 10, 10};
		this->wndlist_listbox->set_size(this->wndlist_listbox, 8, tmp);
	}
	
	this->wndlist->set_visible(this->wndlist, true);
	this->wndlist->focus(this->wndlist);
	
	list_update(this);
}





static void update(struct minircheats * this_, bool ramwatch);
static void set_enabled(struct minircheats * this_, bool enabled)
{
	struct minircheats_impl * this=(struct minircheats_impl*)this_;
	this->enabled=enabled;
	update(this_, true);
}

static bool get_enabled(struct minircheats * this_)
{
	struct minircheats_impl * this=(struct minircheats_impl*)this_;
	return this->enabled;
}

static void update(struct minircheats * this_, bool ramwatch)
{
	struct minircheats_impl * this=(struct minircheats_impl*)this_;
	//Note that various windows should be updated even if cheats are disabled. If the user wants them gone, he can close them.
	if (ramwatch && this->wndsrch_listbox) this->wndsrch_listbox->refresh(this->wndsrch_listbox, (size_t)-1);
	if (!this->enabled) return;
	this->model->cheat_apply(this->model);
}

static void free_(struct minircheats * this_)
{
	struct minircheats_impl * this=(struct minircheats_impl*)this_;
	
	while (this->details) details_free(this->details);
	
	if (this->wndsrch) this->wndsrch->free(this->wndsrch);
	if (this->wndlist) this->wndlist->free(this->wndlist);
	
	this->model->free(this->model);
	
	free(this);
}

struct minircheats * minircheats_create()
{
	struct minircheats_impl * this=malloc(sizeof(struct minircheats_impl));
	memset(this, 0, sizeof(struct minircheats_impl));
	this->i.set_parent=set_parent;
	this->i.set_core=set_core;
	this->i.show_search=show_search;
	this->i.show_list=show_list;
	this->i.set_enabled=set_enabled;
	this->i.get_enabled=get_enabled;
	this->i.update=update;
	//this->i.get_cheat_count=get_cheat_count;
	//this->i.get_cheat=get_cheat;
	//this->i.set_cheat=set_cheat;
	this->i.free=free_;
	
	this->enabled=true;
	this->model=minircheats_create_model();
	this->model->thread_enable(this->model, thread_ideal_count());
	
	return (struct minircheats*)this;
}
