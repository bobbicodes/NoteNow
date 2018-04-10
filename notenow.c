#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

//Widget for Search Bar
typedef struct _SearchBar {
  GtkWidget *search_entry;
  GtkWidget *sbutton;
  GtkWidget *nbutton;
  GtkWidget *qbutton;
  GtkWidget *text_view;
} SearchBar;

//Widget for open-dialog
typedef struct _OpenDialog{
  GtkWidget *window;
  GtkTextBuffer *buffer;
  gchar *filename;
} OpenDialog;

//Widget for new dialog
typedef struct _NewDialog{
  GtkWidget *window;
  GtkWidget *text_view;
  GtkTextBuffer *buffer;
  GtkWidget *statusbar;
} NewDialog;

//Function invoked when save menu item is clickedcol
void save_dialog_selected(GtkWidget *widget, OpenDialog *sdlog){
  GtkWidget *dialog;
  gint response;
  GtkTextBuffer *buffer;

  if (strcmp( gtk_window_get_title(GTK_WINDOW(sdlog->window)) , "Untitled") == 0)
  {

    dialog = gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(sdlog->window),
     GTK_FILE_CHOOSER_ACTION_SAVE,
     GTK_STOCK_SAVE, GTK_RESPONSE_APPLY,
     GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
     NULL);

    response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_APPLY)
    {
     gchar *filename;
     gchar *contents;
     GtkTextIter start, end;
     filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
     buffer = sdlog->buffer;
     gtk_text_buffer_get_bounds(buffer, &start, &end);
     contents = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
     g_file_set_contents(filename, contents, -1, NULL);
     gtk_window_set_title(GTK_WINDOW(sdlog->window), filename);
     sdlog->filename = filename;
   }
   else if (response == GTK_RESPONSE_CANCEL)
   {
     gtk_widget_destroy(dialog);
     return;
   }
   gtk_widget_destroy(dialog);
 }
 else
 {
   GtkTextIter start, end;
   const gchar *filename = gtk_window_get_title(GTK_WINDOW(sdlog->window)); 
   gchar *contents;
   gtk_text_buffer_get_bounds(sdlog->buffer, &start, &end);
   contents = gtk_text_buffer_get_text(sdlog->buffer, &start, &end, FALSE);
   g_file_set_contents(filename, contents, -1, NULL);
 }
}

//Function invoked when new menu item is clicked
void new_dialog_selected(GtkWidget *widget, NewDialog *ndlog){
 gtk_widget_show(ndlog->text_view);
 gtk_widget_show(ndlog->statusbar);
 gtk_window_set_title(GTK_WINDOW(ndlog->window), "Untitled");
 gtk_text_buffer_set_text(ndlog->buffer, "", -1);
}

//Function invoked when open menu item is clicked
void open_dialog_selected(GtkWidget *widget, OpenDialog *odlog){

  GtkWidget *dialog;

  dialog = gtk_file_chooser_dialog_new ("Open File",
    GTK_WINDOW(odlog->window),
    GTK_FILE_CHOOSER_ACTION_OPEN,
    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
    NULL);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
  {
   gchar *filename;
   gchar *contents;
   filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
   g_file_get_contents(filename, &contents, NULL, NULL);
   gtk_text_buffer_set_text(odlog->buffer, contents, -1);
   gtk_window_set_title(GTK_WINDOW(odlog->window), filename);
   odlog->filename=filename;
   g_free (filename);
 }
 gtk_widget_destroy(dialog);
}

//Functions related to search-bar
void find (GtkTextView *text_view, const gchar *text, GtkTextIter *iter)
{
  GtkTextIter mstart, mend;
  gboolean found;
  GtkTextBuffer *buffer;
  GtkTextMark *last_pos;

  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));
  found = gtk_text_iter_forward_search (iter, text, 0, &mstart, &mend, NULL);

  if (found)
  {
    gtk_text_buffer_select_range (buffer, &mstart, &mend);
    last_pos = gtk_text_buffer_create_mark (buffer, "last_pos", 
      &mend, FALSE);
    gtk_text_view_scroll_mark_onscreen (text_view, last_pos);
  }
}

void close_button_clicked (GtkWidget *close_button, SearchBar *sbar)
{
  gtk_widget_hide(sbar->search_entry);
  gtk_widget_hide(sbar->sbutton);
  gtk_widget_hide(sbar->nbutton);
  gtk_widget_hide(sbar->qbutton);
}

void find_menu_selected(GtkWidget *widget, SearchBar *sbar)
{
  gtk_widget_show(sbar->search_entry);
  gtk_widget_show(sbar->sbutton);
  gtk_widget_show(sbar->nbutton);
  gtk_widget_show(sbar->qbutton);
}


void search_button_clicked (GtkWidget *search_button, SearchBar *sbar)
{
  const gchar *text;
  GtkTextBuffer *buffer;
  GtkTextIter iter;

  text = gtk_entry_get_text (GTK_ENTRY (sbar->search_entry));

  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (sbar->text_view));
  gtk_text_buffer_get_start_iter (buffer, &iter);
  
  find (GTK_TEXT_VIEW (sbar->text_view), text, &iter);
}

void next_button_clicked (GtkWidget *next_button, SearchBar *sbar)
{
  const gchar *text;
  GtkTextBuffer *buffer;
  GtkTextMark *last_pos;
  GtkTextIter iter;

  text = gtk_entry_get_text (GTK_ENTRY (sbar->search_entry));
  
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (sbar->text_view));

  last_pos = gtk_text_buffer_get_mark (buffer, "last_pos");
  if (last_pos == NULL)
    return;

  gtk_text_buffer_get_iter_at_mark (buffer, &iter, last_pos);
  find (GTK_TEXT_VIEW (sbar->text_view), text, &iter);
}

//Function invoked when copy option is used
void copy_to_clipboard(GtkWidget *widget, GtkTextBuffer *buffer){
  GtkClipboard *clipboard;
  clipboard = gtk_clipboard_get(GDK_NONE);
  gtk_text_buffer_copy_clipboard(buffer, clipboard);
}

//Function invoked when cut option is used
void cut_to_clipboard(GtkWidget *widget, GtkTextBuffer *buffer){
  GtkClipboard *clipboard;
  clipboard = gtk_clipboard_get(GDK_NONE);
  gtk_text_buffer_cut_clipboard(buffer, clipboard,TRUE);
}

//Function invoked when paste option is used
void paste_from_clipboard(GtkWidget *widget, GtkTextBuffer *buffer){
  GtkClipboard *clipboard;
  clipboard = gtk_clipboard_get(GDK_NONE);
  gtk_text_buffer_paste_clipboard(buffer, clipboard,NULL,TRUE);
}

//Function invoked when text buffer is changed
void update_statusbar(GtkTextBuffer *buffer,
  GtkStatusbar  *statusbar) {
  gchar *msg;
  gint row, col;
  GtkTextIter iter;
  
  gtk_statusbar_pop(statusbar, 0); 

  gtk_text_buffer_get_iter_at_mark(buffer,
    &iter, gtk_text_buffer_get_insert(buffer));

  row = gtk_text_iter_get_line(&iter);
  col = gtk_text_iter_get_line_offset(&iter);

  msg = g_strdup_printf("Col: %d Ln: %d", col+1, row+1);

  gtk_statusbar_push(statusbar, 0, msg);

  g_free(msg);
}

//Function invoked when cursor changes
void mark_set_callback(GtkTextBuffer *buffer, 
  const GtkTextIter *new_location, GtkTextMark *mark, gpointer data) {

  update_statusbar(buffer, GTK_STATUSBAR(data));
}


int main(int argc, char *argv[]) {

  //Definitions

  //Contains definitions for layout related items
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *hbox;

  //Contains Menubar, Menu and menuitems inside it in an iterative way
  GtkWidget *menubar; 
  GtkWidget *fileMenu;
  GtkWidget *editMenu;
  GtkWidget *optionsMenu;
  GtkWidget *fileMi;
  GtkWidget *newMi;
  GtkWidget *openMi;
  GtkWidget *saveMi;
  GtkWidget *quitMi;
  GtkWidget *editMi;
  GtkWidget *cutMi;
  GtkWidget *copyMi;
  GtkWidget *pasteMi;
  GtkWidget *optionsMi;
  GtkWidget *findMi;

  //Contains statusBar definition
  GtkWidget *statusbar;

  //contains Text view and related definitions
  GtkWidget *text_view;
  GtkTextBuffer *buffer;
  GtkWidget* scrolledwindow;

  //searchbar definition
  SearchBar sbar;

  //Miscellaneous declaration
  // GtkClipboard *clipboard;
  GtkAccelGroup *accel_group = NULL;

  //initialize our gtk engine
  gtk_init(&argc, &argv);

  //properties for the main window
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
  gtk_window_set_title(GTK_WINDOW(window), "NoteNow!");

  //Setting up vbox and hbox
  vbox = gtk_vbox_new(FALSE, 2);
  hbox = gtk_hbox_new(FALSE, 2);
  gtk_container_add(GTK_CONTAINER(window), vbox);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

  //initilize accel group which enables shortcut(accelerator) keys
  accel_group = gtk_accel_group_new();
  gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

  //initialize menu bar
  menubar = gtk_menu_bar_new();

  // Setting File Menu //

  //initialize file menu and define file items in it
  fileMenu = gtk_menu_new();
  fileMi = gtk_menu_item_new_with_mnemonic("_File");
  newMi  = gtk_menu_item_new_with_label("New");
  openMi  = gtk_menu_item_new_with_label("Open");
  saveMi  = gtk_menu_item_new_with_label("Save");
  quitMi  = gtk_menu_item_new_with_label("Quit");

  //linking menu and its respective menu items
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), newMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), openMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), saveMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);

  //Defining shortcut keys for menu items
  gtk_widget_add_accelerator(newMi, "activate", accel_group, 
    GDK_n, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 
  gtk_widget_add_accelerator(openMi, "activate", accel_group, 
    GDK_o, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 
  gtk_widget_add_accelerator(saveMi, "activate", accel_group, 
    GDK_s, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 
  gtk_widget_add_accelerator(quitMi, "activate", accel_group, 
    GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 

  //attaching FileMenu to menubar
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi); 

  
  //Setting Edit Menu //

  //initialize edit menu and define file items in it
  editMenu = gtk_menu_new();
  editMi = gtk_menu_item_new_with_mnemonic("_Edit");
  cutMi  = gtk_menu_item_new_with_label("Cut");
  copyMi  = gtk_menu_item_new_with_label("Copy");
  pasteMi  = gtk_menu_item_new_with_label("Paste");

  //linking edit menu and its respective menu items
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(editMi), editMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(editMenu), cutMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(editMenu), copyMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(editMenu), pasteMi);

  //Defining shortcut keys for edit menu items
  gtk_widget_add_accelerator(cutMi, "activate", accel_group, 
    GDK_x, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 
  gtk_widget_add_accelerator(copyMi, "activate", accel_group, 
    GDK_c, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 
  gtk_widget_add_accelerator(pasteMi, "activate", accel_group, 
    GDK_v, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 

  //attaching EditMenu to menubar
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), editMi); 


  //Setting Options Menu //
  
  //initialize options menu and define file items in it
  optionsMenu = gtk_menu_new();
  optionsMi = gtk_menu_item_new_with_mnemonic("_Options");
  findMi  = gtk_menu_item_new_with_label("Find");


  //linking options menu and its respective menu items
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(optionsMi), optionsMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(optionsMenu), findMi);

  //Defining shortcut keys for edit menu items
  gtk_widget_add_accelerator(findMi, "activate", accel_group, 
    GDK_f, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 

  //attaching optionsMenu to menubar
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), optionsMi); 

  //connecting layout of verticalbox to menubar
  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

  //Initializing scrolledwindows
  scrolledwindow = gtk_scrolled_window_new(NULL,NULL);

  //Initializing textviewer and displaying it.
  text_view = gtk_text_view_new ();
  gtk_container_add(GTK_CONTAINER(scrolledwindow),text_view);
  gtk_box_pack_start (GTK_BOX (vbox), scrolledwindow, 1, 1, 0);
  

  // Initializing buffer
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

  //Set up search bar
  sbar.text_view = text_view;
  sbar.search_entry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (hbox), sbar.search_entry, TRUE, TRUE, 0);

  sbar.sbutton = gtk_button_new_with_label ("Search");  
  gtk_box_pack_start (GTK_BOX (hbox),sbar.sbutton, FALSE, FALSE, 0);
  g_signal_connect (G_OBJECT (sbar.sbutton), "clicked", 
    G_CALLBACK (search_button_clicked), &sbar);

  sbar.nbutton = gtk_button_new_with_label ("Next");
  gtk_box_pack_start (GTK_BOX (hbox), sbar.nbutton, FALSE, FALSE, 0);
  g_signal_connect (G_OBJECT (sbar.nbutton), "clicked",
    G_CALLBACK (next_button_clicked), &sbar);

  sbar.qbutton = gtk_button_new_with_label ("Close");
  gtk_box_pack_start (GTK_BOX (hbox), sbar.qbutton, FALSE, FALSE, 0);
  g_signal_connect (G_OBJECT (sbar.qbutton), "clicked",
    G_CALLBACK (close_button_clicked), &sbar);

  //initializing open dialog
  OpenDialog odlog;
  odlog.window = window;
  odlog.buffer= buffer;

  //initializing new dialog
  NewDialog ndlog;
  ndlog.window=window;
  ndlog.buffer = buffer;
  ndlog.text_view = text_view;


  //initializing save dialog
  OpenDialog sdlog;
  sdlog.window=window;
  sdlog.buffer=buffer;

  //Initializing statusbar and displaying it.
  statusbar = gtk_statusbar_new();
  gtk_box_pack_end(GTK_BOX(vbox), statusbar, FALSE, FALSE, 0);

  //initializing status bar to new dialog
  ndlog.statusbar = statusbar;
  
  //connect functions to events that is signalled by gtk

  g_signal_connect(G_OBJECT(window), "destroy",
    G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(quitMi), "activate",
    G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(copyMi),"activate",
    G_CALLBACK(copy_to_clipboard),buffer);

  g_signal_connect(G_OBJECT(cutMi),"activate",
    G_CALLBACK(cut_to_clipboard),buffer);


  g_signal_connect(G_OBJECT(pasteMi),"activate",
    G_CALLBACK(paste_from_clipboard),buffer);


  g_signal_connect(G_OBJECT(findMi),"activate",
    G_CALLBACK(find_menu_selected), &sbar);

  g_signal_connect(G_OBJECT(openMi),"activate",
    G_CALLBACK(open_dialog_selected), &odlog);

  g_signal_connect(G_OBJECT(newMi),"activate",
    G_CALLBACK(new_dialog_selected), &ndlog);

  g_signal_connect(G_OBJECT(saveMi),"activate",
    G_CALLBACK(save_dialog_selected), &sdlog);

  g_signal_connect(buffer, "changed",
    G_CALLBACK(update_statusbar), statusbar);

  g_signal_connect_object(buffer, "mark_set", 
    G_CALLBACK(mark_set_callback), statusbar, 0);

  // display all the windows and enter into event loop
  gtk_widget_show_all(window);

  //Hide find options at the start
  gtk_widget_hide(sbar.search_entry);
  gtk_widget_hide(sbar.sbutton);
  gtk_widget_hide(sbar.nbutton);
  gtk_widget_hide(sbar.qbutton);



  //hide status bar
  gtk_widget_hide(statusbar);



  gtk_main();

  return 0;
}