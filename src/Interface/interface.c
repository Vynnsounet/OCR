#include <gtk/gtk.h>
#include <string.h>


GtkWidget *sudoku;
GtkWidget *image1;
GtkWidget *imagedentrer;
GtkWidget *error;
GtkWidget *rotation;
GtkWidget *gray;
GtkWidget *canny;
GtkWidget *hough;
GtkWidget *flou;
GtkWidget *fixed;
GtkWidget* window;
GtkWidget* second_window;
char * file;
// Signal handler for the "clicked" signal of the start button.
int verif_filename(char *filename, const char *s)
{ 
   int j = strlen(s);
   int i = 0;
   int l = strlen(filename);
   while(i != 3)
   {
       if(filename[l -j] != s[i])
       {
           return 1;
       }
       i+=1;
       j-=1;
   }
   return 0;
}
void on_file_file_set(GtkFileChooserButton *f)
{
    char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(f));
    file = filename;
    int verif = verif_filename(filename, ".png");
    verif += verif_filename(filename, ".bmp");
    verif += verif_filename(filename, ".jpeg");
    verif += verif_filename(filename, ".jpg");
    if(verif == 4)
    {
        if( verif_filename(filename, ".txt") == 0)
        {
            filename = "txt.png";
        }
        else 
        {
            filename = "error.png";
        }
    } 
    int hor = 41;
    int ver = 32;
    if(image1 != NULL)
    {
        gtk_container_remove(GTK_CONTAINER(fixed), image1);
    }
    gtk_widget_hide(sudoku);
    image1 = gtk_image_new_from_file(filename);
    gtk_container_add(GTK_CONTAINER(fixed), image1);
    gtk_widget_show(image1);
    gtk_fixed_move(GTK_FIXED(fixed), image1, hor, ver);
    
}

void on_Help(GtkButton *button, gpointer user_data)
{
    gtk_widget_show_all(second_window);   
}

void on_Start(GtkButton *button, gpointer user_data)
{
    g_print("on_Start()\n");
}

void on_Download(GtkButton *button, gpointer user_data)
{
    
    //save_image(load_image(file), "download");
    g_print("on_Download()\n");
}

void on_Rotation(GtkCheckButton* button, gpointer user_data)
{
    g_print("on_Rotation()\n");
}

void on_Canny(GtkCheckButton* button, gpointer user_data)
{
    g_print("on_Canny()\n");
}

void on_Hough(GtkCheckButton* button, gpointer user_data)
{
    g_print("on_Hough()\n");
}

void on_Flou(GtkCheckButton* button, gpointer user_data)
{
    g_print("on_Flou()\n");
}

void on_Resultat(GtkCheckButton* button, gpointer user_data)
{
    g_print("on_Resultat()\n");
}

void on_Grayscale(GtkCheckButton* button, gpointer user_data)
{
    g_print("on_Grascale()\n");
}


// Main function.
int main (int argc, char *argv[])
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "interface.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    second_window = GTK_WIDGET(gtk_builder_get_object(builder, "second_window"));
    GtkFileChooserButton* file = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "file"));
    GtkButton* Start = GTK_BUTTON(gtk_builder_get_object(builder, "Start"));
    GtkButton* Download = GTK_BUTTON(gtk_builder_get_object(builder, "Download"));
    GtkButton* Help = GTK_BUTTON(gtk_builder_get_object(builder, "Help"));
    GtkCheckButton* Flou = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Flou"));
    GtkCheckButton* Canny = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Canny"));
    GtkCheckButton* Grayscale = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Grayscale"));
    GtkCheckButton* Hough = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Hough"));
    GtkCheckButton* Rotation = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Rotation"));
    GtkCheckButton* Resultat = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Resultat"));
    sudoku = GTK_WIDGET(gtk_builder_get_object(builder, "sudoku"));
    fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));

    // Connects signal handlers.
    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(file, "file_set", G_CALLBACK(on_file_file_set), NULL);
    g_signal_connect(Help, "clicked", G_CALLBACK(on_Help), NULL);
    g_signal_connect(Start, "clicked", G_CALLBACK(on_Start), NULL);
    g_signal_connect(Download, "clicked", G_CALLBACK(on_Download), NULL);
    g_signal_connect(Flou, "toggled", G_CALLBACK(on_Flou), NULL);
    g_signal_connect(Canny, "toggled", G_CALLBACK(on_Canny), NULL);
    g_signal_connect(Resultat, "toggled", G_CALLBACK(on_Resultat), NULL);
    g_signal_connect(Grayscale, "toggled", G_CALLBACK(on_Grayscale), NULL);
    g_signal_connect(Hough, "toggled", G_CALLBACK(on_Hough), NULL);
    g_signal_connect(Rotation, "toggled", G_CALLBACK(on_Rotation), NULL);

    // Runs the main loop.
    gtk_widget_show_all(window);
    image1 = NULL;
    gtk_main();

    // Exits.
    return 0;
}
