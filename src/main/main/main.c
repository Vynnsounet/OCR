#include <gtk/gtk.h>
#include <string.h>
#include "includes/hough.h"
#include "includes/image.h"
#include "includes/image_op.h"
#include "includes/threshold.h"
#include "includes/contrast.h"
#include "includes/Sobel.h"
#include "includes/hough.h"
#include "includes/segmentation.h"
#include "includes/process.h"


GtkWidget *sudoku;
GtkWidget *image1;
GtkWidget *imagedentrer;
GtkWidget *error;
GtkWidget *rot;
GtkWidget *gray;
GtkWidget *canny;
GtkWidget *hough;
GtkWidget *flou;
GtkWidget *fixed;
GtkWidget* window;
GtkWidget* second_window;
GtkWidget* Sudoku;
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
    file = filename;
    resize_image_GTK(load_image(filename), "processed/resized.bmp");
    filename = "processed/resized.bmp";
    image1 = gtk_image_new_from_file(filename);
    gtk_container_add(GTK_CONTAINER(fixed), image1);
    gtk_widget_show(image1);
    gtk_fixed_move(GTK_FIXED(fixed), image1, hor, ver); 
    printf("%s\n", file);
}

void on_Help(GtkButton *button, gpointer user_data)
{

    gtk_widget_show_all(second_window);   
}

void on_Start(GtkButton *button, gpointer user_data)
{
    process(file);
    g_print("on_Start()\n");
}

void on_Download(GtkButton *button, gpointer user_data)
{
    
    
    g_print("on_Download()\n");
}

void on_Rotation(GtkButton* button, gpointer user_data)
{
    g_print("on_Rotation()\n");
}

void on_Sobel(GtkButton* button, gpointer user_data)
{
	int hor = 41;
	int ver = 32;
	if(image1 != NULL)
	{
		gtk_container_remove(GTK_CONTAINER(fixed), image1);
	}
	resize_image_GTK(load_image("processed/sobel.bmp"), "processed/sobel_resized.bmp");
	char *filename = "processed/sobel_resized.bmp";
	image1 = gtk_image_new_from_file(filename);
	gtk_container_add(GTK_CONTAINER(fixed), image1);
	gtk_widget_show(image1);
	gtk_fixed_move(GTK_FIXED(fixed), image1, hor, ver);

	g_print("on_Sobel()\n");
}

void on_Hough(GtkButton* button, gpointer user_data)
{
	int hor = 41;
	int ver = 32;
	if(image1 != NULL)
		{
			gtk_container_remove(GTK_CONTAINER(fixed), image1);
			}
	resize_image_GTK(load_image("processed/hough.bmp"), "processed/hough_resized.bmp");
	char *filename = "processed/hough_resized.bmp";
	image1 = gtk_image_new_from_file(filename);
	gtk_container_add(GTK_CONTAINER(fixed), image1);
	gtk_widget_show(image1);
	gtk_fixed_move(GTK_FIXED(fixed), image1, hor, ver);
	g_print("on_Hough()\n");
}

void on_Flou(GtkButton* button, gpointer user_data)
{
	int hor = 41;
	int ver = 32;
	if(image1 != NULL)
	{
		gtk_container_remove(GTK_CONTAINER(fixed), image1);
	}
	resize_image_GTK(load_image("processed/gauss.bmp"), "processed/gauss_resized.bmp");
	char *filename = "processed/gauss_resized.bmp";
	image1 = gtk_image_new_from_file(filename);
	gtk_container_add(GTK_CONTAINER(fixed), image1);
	gtk_widget_show(image1);
	gtk_fixed_move(GTK_FIXED(fixed), image1, hor, ver);
	g_print("on_Flou()\n");
}

void on_Resultat(GtkButton* button, gpointer user_data)
{
    g_print("on_Resultat()\n");
}

void on_Grayscale(GtkButton* button, gpointer user_data)
{
	int hor = 41;
	int ver = 32;
	if(image1 != NULL)
	{
		gtk_container_remove(GTK_CONTAINER(fixed), image1);
	}
	resize_image_GTK(load_image("processed/grayscale.bmp"), "processed/grayscale_resized.bmp");
	char *filename = "processed/grayscale_resized.bmp";
	image1 = gtk_image_new_from_file(filename);
	gtk_container_add(GTK_CONTAINER(fixed), image1);
	gtk_widget_show(image1);
	gtk_fixed_move(GTK_FIXED(fixed), image1, hor, ver);
	g_print("on_Grayscale()\n");
}

void on_Sudoku(GtkButton* button, gpointer user_data)
{
	int hor = 41;
	int ver = 32;
	if(image1 != NULL)
	{
		gtk_container_remove(GTK_CONTAINER(fixed), image1);
	}
	resize_image_GTK(load_image(file), "processed/resized.bmp");
	char *filename = "processed/resized.bmp";
	image1 = gtk_image_new_from_file(filename);
	gtk_container_add(GTK_CONTAINER(fixed), image1);
	gtk_widget_show(image1);
	gtk_fixed_move(GTK_FIXED(fixed), image1, hor, ver);
	g_print("on_Sudoku()\n)");
}

int main (int argc, char *argv[])
{
    gtk_init(NULL, NULL);

    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "interface.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    second_window = GTK_WIDGET(gtk_builder_get_object(builder, "second_window"));
    GtkFileChooserButton* file = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "file"));
    GtkButton* Start = GTK_BUTTON(gtk_builder_get_object(builder, "Start"));
    GtkButton* Download = GTK_BUTTON(gtk_builder_get_object(builder, "Download"));
    GtkButton* Help = GTK_BUTTON(gtk_builder_get_object(builder, "Help"));
    GtkButton* Flou = GTK_BUTTON(gtk_builder_get_object(builder, "Flou"));
    GtkButton* Canny = GTK_BUTTON(gtk_builder_get_object(builder, "Canny"));
    GtkButton* Grayscale = GTK_BUTTON(gtk_builder_get_object(builder, "Grayscale"));
    GtkButton* Hough = GTK_BUTTON(gtk_builder_get_object(builder, "Hough"));
    GtkButton* Rotation = GTK_BUTTON(gtk_builder_get_object(builder, "Rotation"));
    GtkButton* Resultat = GTK_BUTTON(gtk_builder_get_object(builder, "Resultat"));
    GtkButton* sudo = GTK_BUTTON(gtk_builder_get_object(builder, "Sudoku"));

    sudoku = GTK_WIDGET(gtk_builder_get_object(builder, "sudoku"));
    fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));

    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(file, "file_set", G_CALLBACK(on_file_file_set), NULL);
    g_signal_connect(Help, "clicked", G_CALLBACK(on_Help), NULL);
    g_signal_connect(Start, "clicked", G_CALLBACK(on_Start), NULL);
    g_signal_connect(Download, "clicked", G_CALLBACK(on_Download), NULL);
    g_signal_connect(Flou, "clicked", G_CALLBACK(on_Flou), NULL);
    g_signal_connect(Canny, "clicked", G_CALLBACK(on_Sobel), NULL);
    g_signal_connect(Resultat, "clicked", G_CALLBACK(on_Resultat), NULL);
    g_signal_connect(Grayscale, "clicked", G_CALLBACK(on_Grayscale), NULL);
    g_signal_connect(Hough, "clicked", G_CALLBACK(on_Hough), NULL);
    g_signal_connect(Rotation, "clicked", G_CALLBACK(on_Rotation), NULL);
    g_signal_connect(sudo, "clicked", G_CALLBACK(on_Sudoku), NULL);

    gtk_widget_show_all(window);
    image1 = NULL;
    gtk_main();

    return 0;
}
