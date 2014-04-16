#include <iostream>
#include <gtk/gtk.h>
#include "Neuron.h"
#include "Net.h"
#include <math.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

struct
{
    Net *network;
    GtkWidget *window;
} programData;

struct
{
    int data25[24] = { 0x4547F1, 0x1E8FA3E, 0xF8420F, 0x1E8C63E, 0x1F87A1F, 0x1F87A10, 0xF85E2F, 0x118FE31, 0x421084, 0x1F0862E,
                       0x1197251, 0x108421F, 0x1BAC631, 0x11CD671, 0xE8C62E, 0x1E8FA10, 0x1E8FA31, 0xF8383E, 0x1F21084, 0x118C62E,
                       0x118C544, 0x1151151, 0x1151084, 0x1F1111F
                     };
    long long int data49[24] = { 0x71B36C7FFFE3, 0x1F3FE3F98FFFC, 0xFFFE0C183FBF, 0x1FBFE3C78FFFE, 0x1FFFE0FD83FFF, 0x1FFFE0FD83060,
                                 0xFFFE0DF8FFBF, 0x18F1E3FF8F1E3, 0x204081020408, 0x1FFF83078FFBE, 0x18F36CF1B3363, 0x183060C183FFF, 0x18FBFFD78F1E3,
                                 0x18F9FBD7BF3E3, 0xFBFE3C78FFBE, 0x1FBFE3FFFB060, 0x1FBFE3FFFB1E3, 0xFFFE07C0FFFE, 0x1FFF881020408, 0x18F1E3C78FFBE,
                                 0x18F1E3C6D9B1C, 0x18FB9C1073BE3, 0x18FBBE3820408, 0x1FFF8638C3FFF
                               };
} letters;

static gboolean
draw_letter (GtkWidget *da,
             cairo_t   *cr,
             gpointer   data);

static gboolean
draw_selected_letter (GtkWidget *da,
                      cairo_t   *cr,
                      gpointer   data);

//create network
extern "C" __declspec(dllexport)
void on_button1_clicked(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = GTK_BUILDER(data);
    GtkRadioButton *b1 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton1"));
    GtkRadioButton *b2 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton2"));
    GtkRadioButton *b3 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton3"));
    GtkWidget *trainButton = GTK_WIDGET(gtk_builder_get_object(builder, "button2"));
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b1)))
    {
        programData.network = new Net(25);
        g_print("Vytvorena siet 5x5\n");
    }
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b2)))
    {
        programData.network = new Net(49);
        g_print("Vytvorena siet 7x7\n");
    }
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b3)))
    {
        programData.network = new Net(81);
        g_print("Vytvorena siet 9x9\n");
    }
    gtk_widget_set_sensitive(trainButton, true);


}

//train network
extern "C" __declspec(dllexport)
void on_button2_clicked(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = GTK_BUILDER(data);
    GtkRadioButton *b1 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton1"));
    GtkRadioButton *b2 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton2"));
    GtkRadioButton *b3 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton3"));
    GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(gtk_builder_get_object (builder, "treeview3")));
    GtkWidget *testButton = GTK_WIDGET(gtk_builder_get_object(builder, "button3"));

    GList *list1 = gtk_tree_selection_get_selected_rows(sel, NULL);
    vector < vector < int > > trainset;
    vector <int> subset1;
    long long int cislo;
    int sizee;
    for(list1; list1!=NULL; list1=list1->next)
    {
        cout<<gtk_tree_path_get_indices ((GtkTreePath *)list1->data)[0]<<endl;
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b1)))
        {
            cislo = letters.data25[gtk_tree_path_get_indices ((GtkTreePath *)list1->data)[0]];
            sizee=25;
            g_print("radiobutton1\n");
        }
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b2)))
        {
            cislo = letters.data49[gtk_tree_path_get_indices ((GtkTreePath *)list1->data)[0]];
            sizee=49;
            g_print("radiobutton2\n");
        }
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b3)))
        {
            g_print("radiobutton3\n");
        }

        subset1.clear();
        for(int i=0; i<sizee; i++)
        {
            if(cislo==0) subset1.push_back(-1);
            if(cislo%2==0) subset1.push_back(-1);
            else subset1.push_back(1);
            cislo /= 2;
        }
        trainset.push_back(subset1);

    }

    programData.network->train(trainset);

    gtk_widget_set_sensitive(testButton, true);

    /*for(int i=0; i<programData.network->getSize(); i++)
    {
        for(int j=0; j<programData.network->getSize(); j++)
        {
            cout<<programData.network->GetNeurons()[i]->getWeight(j)<<endl;
        }
    }*/

}

//test network
extern "C" __declspec(dllexport)
void on_button3_clicked(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = GTK_BUILDER(data);
    GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(gtk_builder_get_object (builder, "treeview3")));
    GtkRadioButton *b1 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton1"));
    GtkRadioButton *b2 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton2"));
    GtkRadioButton *b3 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton3"));
    GtkDrawingArea *da = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "drawingarea2"));

    GList *list1 = gtk_tree_selection_get_selected_rows(sel, NULL);
    int sizee;
    vector <int> subset1;
    long long int cislo;

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b1)))
    {
        cislo = letters.data25[gtk_tree_path_get_indices ((GtkTreePath *)list1->data)[0]];
        sizee = 25;
        g_print("radiobutton1\n");
    }
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b2)))
    {
        cislo = letters.data49[gtk_tree_path_get_indices ((GtkTreePath *)list1->data)[0]];
        sizee = 49;
        g_print("radiobutton2\n");
    }
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b3)))
    {
        g_print("radiobutton3\n");
    }
    subset1.clear();
    for(int i=0; i<sizee; i++)
    {
        if(cislo==0) subset1.push_back(-1);
        if(cislo%2==0) subset1.push_back(-1);
        else subset1.push_back(1);
        cislo /= 2;
    }
    programData.network->test(subset1);
    for(int i=programData.network->getSize()-1; i>=0; i--)
    {
        if(programData.network->GetNeurons()[i]->getState() == -1) cout<<0;
        else cout<<1;
    }

    cout << endl;

    g_signal_connect (da, "draw", G_CALLBACK (draw_letter), (gpointer)programData.network->getSize());
    gtk_widget_queue_draw(GTK_WIDGET(da));
}

//draw recognized letter
static gboolean
draw_letter (GtkWidget *da,
             cairo_t   *cr,
             gpointer   data)
{
    int sizee = (int)sqrt((int) data);
    cout<<sizee<<endl;
    for(int i=programData.network->getSize()-1, x=50, y= 50; i>=0; i--)
    {
        if(programData.network->GetNeurons()[i]->getState() == -1)
        {
            cairo_rectangle(cr, x,y,20,20);
            cairo_stroke(cr);
        }
        else
        {
            cairo_rectangle(cr, x,y,20,20);
            cairo_stroke_preserve(cr);
            cairo_fill(cr);

        }
        if(x-50 < (sizee-1)*21) x+=21;
        else
        {
            x= 50;
            y+=21;
        }
    }
}

extern "C" __declspec(dllexport)
void on_treeview3_cursor_changed (GtkTreeView *tree_view,
                                  gpointer     data)
{
    GtkBuilder *builder = GTK_BUILDER(data);
    GtkDrawingArea *da = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "drawingarea1"));
    GtkRadioButton *b1 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton1"));
    GtkRadioButton *b2 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton2"));
    GtkRadioButton *b3 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton3"));

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b1)))
    {
        g_print("radiobutton1\n");
    }
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b2)))
    {
        g_print("radiobutton2\n");
    }
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b3)))
    {
        g_print("radiobutton3\n");
    }

    g_signal_connect (da, "draw", G_CALLBACK (draw_selected_letter), data);
    gtk_widget_queue_draw(GTK_WIDGET(da));
}

//draw selected letter from treeview
static gboolean
draw_selected_letter (GtkWidget *da, cairo_t   *cr, gpointer   data)
{
    cout<<"tu"<<endl;
    GtkBuilder *builder = GTK_BUILDER(data);
    GtkRadioButton *b1 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton1"));
    GtkRadioButton *b2 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton2"));
    GtkRadioButton *b3 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton3"));
    GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(gtk_builder_get_object (builder, "treeview3")));
    GList *list1 = gtk_tree_selection_get_selected_rows(sel, NULL);
    if(list1==NULL) return FALSE;
    vector <int> subset1;
    long long int cislo;

    int sizee;
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b1)))
    {
        cislo = letters.data25[gtk_tree_path_get_indices ((GtkTreePath *)list1->data)[0]];
        sizee=5;
    }
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b2)))
    {
        cislo = letters.data49[gtk_tree_path_get_indices ((GtkTreePath *)list1->data)[0]];
        sizee=7;
    }
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b3)))
    {
        sizee=9;
    }

    subset1.clear();
    for(int i=0; i<sizee*sizee; i++)
    {
        if(cislo==0) subset1.push_back(-1);
        if(cislo%2==0) subset1.push_back(-1);
        else subset1.push_back(1);
        cislo /= 2;
    }

    for(int i=sizee*sizee-1, x=50, y= 50; i>=0; i--)
    {
        if(subset1[i] == -1)
        {
            cairo_rectangle(cr, x,y,20,20);
            cairo_stroke(cr);
        }
        else
        {
            cairo_rectangle(cr, x,y,20,20);
            cairo_stroke_preserve(cr);
            cairo_fill(cr);

        }
        if(x-50 < (sizee-1)*21) x+=21;
        else
        {
            x= 50;
            y+=21;
        }
    }
}

vector< int >  random_demage(int max_rand, int how_many)
{
    srand(time(NULL));
    int num;
    vector <int> indices;
    for(int i=0; i<how_many; i++)
    {
        num = rand() % max_rand;
        //cout<<num<<endl;
        indices.push_back(num);
    }
    sort(indices.begin(), indices.end());
    for(int i=0; i<how_many; i++)
    {
        cout<<indices[i]<<endl;
    }
    return indices;
}

int main(int argc, char *argv[])
{
    random_demage(25, 13);
    GtkBuilder      *builder;
    GtkWidget       *window;

    gtk_init (&argc, &argv);

    builder = gtk_builder_new ();

    gtk_builder_add_from_file (builder, "hopfield.glade", NULL);
    window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
    gtk_builder_connect_signals (builder, (gpointer)builder);

    GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(gtk_builder_get_object (builder, "treeview3")));
    gtk_tree_selection_set_mode(sel, GTK_SELECTION_MULTIPLE);

    gtk_widget_show (window);

    gtk_main ();
    g_object_unref (G_OBJECT (builder));

    /*Net *network = new Net(25);
    vector < vector < int > > trainset;
    vector <int> subset1;
    int data25[] = { 0x1E8FA3E, 0x1E8C63E };
    int cislo;

    for(int j=0; j<2; j++)
    {
        cislo = data25[j];
        subset1.clear();
        for(int i=0; i<25; i++)
        {
            if(cislo==0) subset1.push_back(-1);
            if(cislo%2==0) subset1.push_back(-1);
            else subset1.push_back(1);
            cislo /= 2;
        }
        trainset.push_back(subset1);
    }*/

    /*for(int i=0; i<network->getSize(); i++)
    {
        for(int j=0; j<network->getSize(); j++)
        {
            cout<<network->GetNeurons()[i]->getWeight(j)<<endl;
        }
    }*/

    //network->train(trainset);

    /*for(int i=0; i<network->getSize(); i++)
    {
        for(int j=0; j<network->getSize(); j++)
        {
            cout<<network->GetNeurons()[i]->getWeight(j)<<endl;
        }
    }
    for(int i=0; i<network->getSize(); i++)
    {
        cout<<network->GetNeurons()[i]->getState()<<endl;
    }*/

    //network->test(subset1);

    /*for(int i=network->getSize()-1; i>=0; i--)
    {
        if(network->GetNeurons()[i]->getState() == -1) cout<<0;
        else cout<<1;
    }

    cout << endl;*/
    return 0;
}
