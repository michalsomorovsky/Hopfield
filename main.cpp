#include <iostream>
#include <gtk/gtk.h>
#include "Neuron.h"
#include "Net.h"

using namespace std;

extern "C" __declspec(dllexport)
void on_button1_clicked(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = GTK_BUILDER(data);
    GtkRadioButton *b1 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton1"));
    GtkRadioButton *b2 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton2"));
    GtkRadioButton *b3 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton3"));
    b1->get_active();
    g_print("lama");
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

    gtk_widget_show (window);

    gtk_main ();
    g_object_unref (G_OBJECT (builder));
    Net *network = new Net(5);
    vector < vector < int > > trainset;
    vector <int> subset1;

    subset1.clear();
    subset1.push_back(-1);
    subset1.push_back(1);
    subset1.push_back(-1);
    subset1.push_back(1);
    subset1.push_back(-1);
    trainset.push_back(subset1);
    subset1.clear();
    subset1.push_back(1);
    subset1.push_back(1);
    subset1.push_back(1);
    subset1.push_back(-1);
    subset1.push_back(-1);
    trainset.push_back(subset1);
    subset1.clear();
    subset1.push_back(-1);
    subset1.push_back(-1);
    subset1.push_back(1);
    subset1.push_back(1);
    subset1.push_back(1);
    trainset.push_back(subset1);

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

    for(int i=0; i<network->getSize(); i++)
    {
        cout<<network->GetNeurons()[i]->getState()<<endl;
    }

    cout << "Hello world!" << endl;
    return 0;
}
