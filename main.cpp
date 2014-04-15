#include <iostream>
#include <gtk/gtk.h>
#include "Neuron.h"
#include "Net.h"

using namespace std;

struct
{
    Net *network;
    GtkWidget *window;
} programData;

extern "C" __declspec(dllexport)
void on_button1_clicked(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = GTK_BUILDER(data);
    GtkRadioButton *b1 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton1"));
    GtkRadioButton *b2 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton2"));
    GtkRadioButton *b3 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton3"));
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b1)))
    {
        programData.network = new Net(25);
        g_print("radiobutton1\n");
    }
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b2)))
    {
        programData.network = new Net(49);
        g_print("radiobutton2\n");
    }
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b3)))
    {
        programData.network = new Net(81);
        g_print("radiobutton3\n");
    }

}
extern "C" __declspec(dllexport)
void on_button2_clicked(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = GTK_BUILDER(data);
    GtkRadioButton *b1 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton1"));
    GtkRadioButton *b2 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton2"));
    GtkRadioButton *b3 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton3"));
    GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(gtk_builder_get_object (builder, "treeview3")));

    GList *list1 = gtk_tree_selection_get_selected_rows(sel, NULL);
    do
    {
        cout<<gtk_tree_path_get_indices ((GtkTreePath *)list1->data)[0]<<endl;
    } while ((list1 = list1->next) != NULL);


    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b1)))
    {
        programData.network = new Net(25);
        g_print("radiobutton1\n");
    }
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b2)))
    {
        programData.network = new Net(49);
        g_print("radiobutton2\n");
    }
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b3)))
    {
        programData.network = new Net(81);
        g_print("radiobutton3\n");
    }
}

int main(int argc, char *argv[])
{
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

    Net *network = new Net(25);
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
    }

    /*for(int i=0; i<network->getSize(); i++)
    {
        for(int j=0; j<network->getSize(); j++)
        {
            cout<<network->GetNeurons()[i]->getWeight(j)<<endl;
        }
    }*/

    network->train(trainset);

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

    network->test(subset1);

    for(int i=network->getSize()-1; i>=0; i--)
    {
        if(network->GetNeurons()[i]->getState() == -1) cout<<0;
        else cout<<1;
    }

    cout << endl;
    return 0;
}
