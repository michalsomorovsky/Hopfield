#include <iostream>
#include <gtk/gtk.h>
#include "Neuron.h"
#include "Net.h"
#include <math.h>
#include <stdlib.h>
#include <algorithm>

#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif // WIN32

using namespace std;

struct
{
    Net *network;
    GtkWindow *window;
} programData;

struct
{
    //data for 5x5 network from A to Z
    string data25[24] = { "4547F1", "1E8FA3E", "F8420F", "1E8C63E", "1F87A1F", "1F87A10", "F85E2F", "118FE31", "421084", "1F0862E",
                          "1197251", "108421F", "1BAC631", "11CD671", "E8C62E", "1E8FA10", "1E8FA31", "F8383E", "1F21084", "118C62E",
                          "118C544", "1151151", "1151084", "1F1111F"
                        };
    //data for 7x7 network from A to Z
    string data49[24] = { "71B36C7FFFE3", "1F3FE3F98FFFC", "FFFE0C183FBF", "1FBFE3C78FFFE", "1FFFE0FD83FFF", "1FFFE0FD83060",
                          "FFFE0DF8FFBF", "18F1E3FF8F1E3", "204081020408", "1FFF83078FFBE", "18F36CF1B3363", "183060C183FFF", "18FBFFD78F1E3",
                          "18F9FBD7BF3E3", "FBFE3C78FFBE", "1FBFE3FFFB060", "1FBFE3FFFB1E3", "FFFE07C0FFFE", "1FFF881020408", "18F1E3C78FFBE",
                          "18F1E3C6D9B1C", "18FB9C1073BE3", "18FBBE3820408", "1FFF8638C3FFF"
                        };
    //data for 9x9 network from A to Z
    string data81[24] = { "381C3B9DD83FFFFF0783", "1FCFE60F07FCC1E0FF9FC", "7F3FE030180C0600FE7F", "1FCFE60F0783C1E0FF9FC", "1FFFFE0301FCC0603FFFF", "1FFFFE0301FCC06030180",
                          "7F3FE03019FC1E0CFE7F", "183C1E0F07FFFFE0F0783", "10080402010080402010", "1FFFF80C0783C1E0CF87C", "183C3E7BF1E0FC67B0F83", "180C06030180C0603FFFF",
                          "183F7FFF2783C1E0F0783", "183E1F8F6793CDE3F0F83", "7C3E60F0783C1E0CF87C", "1FCFE60F07FCFE6030180", "1FCFE60F07FCFE60F0783", "7F3FE03007C0180FF9FC",
                          "1FFFF8402010080402010", "183C1E0F0783C1E0CF87C", "183C1E0F0783773B87038", "183C19B0D810361B30783", "183C19B0D810080402010", "1FFFF818183830303FFFF"
                        };
} letters;

vector<int> selectedLetter;

static gboolean draw_letter (GtkWidget *da, cairo_t   *cr, gpointer   data);

static gboolean draw_selected_letter (GtkWidget *da, cairo_t   *cr, gpointer   data);

vector< int >  random_damage(int max_rand, int how_many);

string hexstringtobinstring(string hex);

//create network
extern "C" DLLEXPORT //__declspec(dllexport)
void on_button1_clicked(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = GTK_BUILDER(data);
    GtkRadioButton *b1 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton1"));
    GtkRadioButton *b2 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton2"));
    GtkRadioButton *b3 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton3"));
    GtkWidget *trainButton = GTK_WIDGET(gtk_builder_get_object(builder, "button2"));

    string message;
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b1)))
    {
        programData.network = new Net(25);
        message = "Vytvorena siet 5x5";
    }
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b2)))
    {
        programData.network = new Net(49);
        message = "Vytvorena siet 7x7";
    }
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b3)))
    {
        programData.network = new Net(81);
        message = "Vytvorena siet 9x9";
    }
    gtk_widget_set_sensitive(trainButton, true);

    GtkWidget* dialog = gtk_message_dialog_new (programData.window,
                        GTK_DIALOG_DESTROY_WITH_PARENT,
                        GTK_MESSAGE_INFO,
                        GTK_BUTTONS_OK,
                        message.c_str());
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy(dialog);
}

//train network
extern "C" DLLEXPORT //__declspec(dllexport)
void on_button2_clicked(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = GTK_BUILDER(data);
    GtkRadioButton *b1 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton1"));
    GtkRadioButton *b2 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton2"));
    GtkRadioButton *b3 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton3"));
    GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(gtk_builder_get_object (builder, "treeview3")));
    GtkWidget *testButton = GTK_WIDGET(gtk_builder_get_object(builder, "button3"));
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(gtk_builder_get_object (builder, "treeview3")));
    GtkTreeModel *model2 = gtk_tree_view_get_model(GTK_TREE_VIEW(gtk_builder_get_object (builder, "treeview1")));
    GtkTreeIter iter;
    GtkTreeIter iter2;
    gchar *value;
    vector < vector < int > > trainset;
    vector <int> subset1;
    string cislo2;
    int sizee;

    GList *list1 = gtk_tree_selection_get_selected_rows(sel, &model);

    gtk_list_store_clear(GTK_LIST_STORE(model2));

    for(; list1!=NULL; list1=list1->next)
    {
        //get selected letter and add it to list of selected letters
        gtk_tree_model_get_iter(model, &iter, (GtkTreePath *)list1->data);
        gtk_tree_model_get(model, &iter, 0, &value, -1);
        gtk_list_store_append(GTK_LIST_STORE(model2), &iter2);
        gtk_list_store_set(GTK_LIST_STORE(model2), &iter2, 0, value, -1);

        //choose letter representation according to size of network
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b1)))
        {
            cislo2 = hexstringtobinstring(letters.data25[gtk_tree_path_get_indices ((GtkTreePath *)list1->data)[0]]);
            sizee=25;
        }
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b2)))
        {
            cislo2 = hexstringtobinstring(letters.data49[gtk_tree_path_get_indices ((GtkTreePath *)list1->data)[0]]);
            sizee=49;
        }
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b3)))
        {
            cislo2 = hexstringtobinstring(letters.data81[gtk_tree_path_get_indices ((GtkTreePath *)list1->data)[0]]);
            sizee=81;
        }

        subset1.clear();

        //construct train set
        for(int i=0; i<cislo2.size(); i++)
        {
            if(cislo2[i]=='0') subset1.push_back(-1);
            else subset1.push_back(1);
        }
        trainset.push_back(subset1);
    }

    //train network with selected letters
    programData.network->train(trainset);

    gtk_widget_set_sensitive(testButton, true);
}

//test network
extern "C" DLLEXPORT //__declspec(dllexport)
void on_button3_clicked(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = GTK_BUILDER(data);
    GtkDrawingArea *da = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "drawingarea2"));

    //test network with selected letter
    programData.network->test(selectedLetter);

    g_signal_connect (da, "draw", G_CALLBACK (draw_letter), (gpointer)programData.network->getSize());
    gtk_widget_queue_draw(GTK_WIDGET(da));
}

//draw recognized letter
static gboolean
draw_letter (GtkWidget *da,
             cairo_t   *cr,
             gpointer   data)
{
    int sizee = (int)sqrt(programData.network->getSize());

    for(int i=0, x=50, y= 50; i<programData.network->getSize(); i++)
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
    return TRUE;
}

//redraw selected letter from treeview
extern "C" DLLEXPORT //__declspec(dllexport)
void on_treeview3_cursor_changed (GtkTreeView *tree_view,
                                  gpointer     data)
{
    GtkBuilder *builder = GTK_BUILDER(data);
    GtkDrawingArea *da = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "drawingarea1"));

    g_signal_connect (da, "draw", G_CALLBACK (draw_selected_letter), data);
    gtk_widget_queue_draw(GTK_WIDGET(da));
}

//draw selected letter from treeview
static gboolean
draw_selected_letter (GtkWidget *da, cairo_t   *cr, gpointer   data)
{
    GtkBuilder *builder = GTK_BUILDER(data);
    GtkRadioButton *b1 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton1"));
    GtkRadioButton *b2 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton2"));
    GtkRadioButton *b3 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton3"));
    GtkRadioButton *db1 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton4"));
    GtkRadioButton *db2 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton5"));
    GtkRadioButton *db3 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton6"));
    GtkRadioButton *db4 = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton7"));
    GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(gtk_builder_get_object (builder, "treeview3")));
    vector <int> subset1;
    string cislo2;
    int sizee;

    //get list of selected rows
    GList *list1 = gtk_tree_selection_get_selected_rows(sel, NULL);

    //nothing was selected
    if(list1==NULL) return FALSE;


    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b1)))
    {
        cislo2 = hexstringtobinstring(letters.data25[gtk_tree_path_get_indices ((GtkTreePath *)list1->data)[0]]);
        sizee=5;
    }
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b2)))
    {
        cislo2 = hexstringtobinstring(letters.data49[gtk_tree_path_get_indices ((GtkTreePath *)list1->data)[0]]);
        sizee=7;
    }
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b3)))
    {
        cislo2 = hexstringtobinstring(letters.data81[gtk_tree_path_get_indices ((GtkTreePath *)list1->data)[0]]);
        sizee=9;
    }
    vector <int> damage;

    //generating random damage of selected letter
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(db2)))    //10% damage
    {
        damage = random_damage(sizee*sizee, (sizee*sizee)/10);
    }
    else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(db3)))   //20% damage
    {
        damage = random_damage(sizee*sizee, (sizee*sizee)/5);
    }
    else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(db4)))   //50% damage
    {
        damage = random_damage(sizee*sizee, (sizee*sizee)/2);
    }

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(db1)))    //no damage was selected
    {
        subset1.clear();
        for(int i=0; i<cislo2.size(); i++)
        {
            if(cislo2[i]=='0') subset1.push_back(-1);
            else subset1.push_back(1);
        }
    }
    else
    {
        subset1.clear();
        for(int i=0, j=0; i<cislo2.size(); i++)
        {
            if(i == damage[j])
            {
                if(cislo2[i]=='0') subset1.push_back(1);
                else subset1.push_back(-1);
                j++;
            }
            else
            {
                if(cislo2[i]=='0') subset1.push_back(-1);
                else subset1.push_back(1);
            }
        }
    }
    selectedLetter = subset1;

    //drawing selected letter
    for(int i=0, x=50, y= 50; i<sizee*sizee; i++)
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
    return TRUE;
}

//generate vector of random numbers from 0 to max_rand
vector< int >  random_damage(int max_rand, int how_many)
{
    srand(time(NULL));
    int num;
    vector <int> indices;

    //generating vector of unique random numbers sorted
    while(indices.size()<how_many)
    {
        for(int i=indices.size(); i<how_many; i++)
        {
            num = rand() % max_rand;
            indices.push_back(num);
        }
        sort(indices.begin(), indices.end());
        indices.erase(unique(indices.begin(), indices.end()), indices.end());
    }
    return indices;
}

//changing size of network. new network has to be created
extern "C" DLLEXPORT //__declspec(dllexport)
void on_radiobutton1_toggled (GtkToggleButton *togglebutton, gpointer data)
{
    GtkBuilder *builder = GTK_BUILDER(data);
    GtkDrawingArea *da = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "drawingarea1"));
    GtkWidget *testButton = GTK_WIDGET(gtk_builder_get_object(builder, "button3"));
    GtkWidget *trainButton = GTK_WIDGET(gtk_builder_get_object(builder, "button2"));

    gtk_widget_queue_draw(GTK_WIDGET(da));
    gtk_widget_set_sensitive(testButton, false);
    gtk_widget_set_sensitive(trainButton, false);

}

//changing a damage of letter
extern "C" DLLEXPORT //__declspec(dllexport)
void on_radiobutton4_toggled (GtkToggleButton *togglebutton, gpointer data)
{
    GtkBuilder *builder = GTK_BUILDER(data);
    GtkDrawingArea *da = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "drawingarea1"));
    gtk_widget_queue_draw(GTK_WIDGET(da));
}

//convert hexa string to binary representation
string hexstringtobinstring(string hex)
{
    string bin;
    for(int i=0; i<hex.size(); i++)
    {
        switch(hex[i])
        {
        case '0':
            bin += "0000";
            break;
        case '1':
            bin+= "0001";
            break;
        case '2':
            bin+="0010";
            break;
        case '3':
            bin+="0011";
            break;
        case '4':
            bin+="0100";
            break;
        case '5':
            bin+="0101";
            break;
        case '6':
            bin+="0110";
            break;
        case '7':
            bin+="0111";
            break;
        case '8':
            bin+="1000";
            break;
        case '9':
            bin+="1001";
            break;
        case 'A':
            bin+="1010";
            break;
        case 'B':
            bin+="1011";
            break;
        case 'C':
            bin+="1100";
            break;
        case 'D':
            bin+="1101";
            break;
        case 'E':
            bin+="1110";
            break;
        case 'F':
            bin+="1111";
            break;
        }
    }

    //aligning, add or trim leading zeros
    if(bin.size() == 80 || bin.size() == 48 ||bin.size() == 24) return "0"+bin;
    else if(bin.size() == 84 || bin.size() == 52 || bin.size() == 28) return bin.substr(3, string::npos);

    return bin;
}

//main loop
int main(int argc, char *argv[])
{
    GtkBuilder      *builder;
    GtkWidget       *window;

    gtk_init (&argc, &argv);

    builder = gtk_builder_new ();

    gtk_builder_add_from_file (builder, "hopfield.glade", NULL);
    window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
    gtk_builder_connect_signals (builder, (gpointer)builder);

    programData.window = GTK_WINDOW(window);

    GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(gtk_builder_get_object (builder, "treeview3")));
    GtkTreeSelection *sel2 = gtk_tree_view_get_selection(GTK_TREE_VIEW(gtk_builder_get_object (builder, "treeview1")));
    gtk_tree_selection_set_mode(sel, GTK_SELECTION_MULTIPLE);
    gtk_tree_selection_set_mode(sel2, GTK_SELECTION_NONE);

    gtk_widget_show (window);

    gtk_main ();
    g_object_unref (G_OBJECT (builder));

    return 0;
}
