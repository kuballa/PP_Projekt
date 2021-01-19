#include <gtk/gtk.h>

typedef struct {
    GtkWidget *w_dlg_fileimg_choose;    // wskaźnik do file chooser dialog box
    GtkWidget *w_img_main;              // wskaźnik do image widget
    GtkWidget *w_txtvw_main;            // wskaźnik do text view object
    GtkWidget *w_dlg_file_choose;       // wskaźnik do file chooser dialog box
    GtkTextBuffer *textbuffer_main;     // wskaźnik do text buffer
    GtkWidget *w_dlg_about;             // wskaźnik do about dialog
} app_widgets;

int main(int argc, char *argv[])
{
    GtkBuilder      *builder;
    GtkWidget       *window;
    app_widgets     *widgets = g_slice_new(app_widgets);

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("glade/window_main.glade");

    //wskaźniki do widgetów
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    widgets->w_dlg_fileimg_choose = GTK_WIDGET(gtk_builder_get_object(builder, "dlg_fileimg_choose"));
    widgets->w_img_main = GTK_WIDGET(gtk_builder_get_object(builder, "img_main"));
    widgets->w_txtvw_main = GTK_WIDGET(gtk_builder_get_object(builder, "txtvw_main"));
    widgets->w_dlg_file_choose = GTK_WIDGET(gtk_builder_get_object(builder, "dlg_file_choose"));
    widgets->textbuffer_main = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "textbuffer_main"));
    widgets->w_dlg_about = GTK_WIDGET(gtk_builder_get_object(builder, "dlg_about"));

    
    gtk_builder_connect_signals(builder, widgets);

    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();
    g_slice_free(app_widgets, widgets);

    return 0;
}

// File -> Open image
void on_menuitm_openimg_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts)
{
    gchar *file_name = NULL;        // Name of file to open from dialog box
    
    // Pokaż "Open Image"
    gtk_widget_show(app_wdgts->w_dlg_fileimg_choose);
    
    // Sprawdź wartość return z Open Image dialog box aby zobaczyć czy użytkownik nacisnął Open button
    if (gtk_dialog_run(GTK_DIALOG (app_wdgts->w_dlg_fileimg_choose)) == GTK_RESPONSE_OK) {
        // Pobierz nazwę pliku z wyszukiwarki plików
        file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(app_wdgts->w_dlg_fileimg_choose));
        if (file_name != NULL) {
            gtk_image_set_from_file(GTK_IMAGE(app_wdgts->w_img_main), file_name);
        }
        g_free(file_name);
    }

    // Jeżeli skończyłeś pracę z wyszukiwarką plików, w tedy możesz już ją zamknąć
    gtk_widget_hide(app_wdgts->w_dlg_fileimg_choose);
}
// File --> Open text
void on_menuitm_open_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts)
{
    gchar *file_name = NULL;        // nazwa pliku którą chcemy pobrać z wyszukiwarki plików
    gchar *file_contents = NULL;    // czytanie zawartości pliku
    gboolean file_success = FALSE;  // status pliku
   
    gtk_widget_show(app_wdgts->w_dlg_file_choose);
    
    if (gtk_dialog_run(GTK_DIALOG (app_wdgts->w_dlg_file_choose)) == GTK_RESPONSE_OK) {
        // pobierz nazwę pliku
        file_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(app_wdgts->w_dlg_file_choose));
        if (file_name != NULL) {
            // pobierz zawartość do dynamicznie zaalokowaniej pamięci
            file_success = g_file_get_contents(file_name, &file_contents, NULL, NULL);
            if (file_success) {
		    
                gtk_text_buffer_set_text(app_wdgts->textbuffer_main, file_contents, -1);
            }
            g_free(file_contents);
        }
        g_free(file_name);
    }

    // Jeżeli skończyłeś pracę z wyszukiwarką plików, w tedy możesz już ją zamknąć
    gtk_widget_hide(app_wdgts->w_dlg_file_choose);
}

// Help -> About
void on_menuitm_about_activate(GtkMenuItem *menuitem, app_widgets *app_wdgts)
{
    gtk_widget_show(app_wdgts->w_dlg_about);
}

// Przycisk zamykania About
void on_dlg_about_response(GtkDialog *dialog, gint response_id, app_widgets *app_wdgts)
{
    gtk_widget_hide(app_wdgts->w_dlg_about);
}

// Wywoływane kiedy zamykamy aplikację
void on_window_main_destroy()
{
    gtk_main_quit();
}
