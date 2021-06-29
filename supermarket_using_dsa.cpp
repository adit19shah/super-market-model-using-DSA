/* C++ Software for a Super-Market Scenario

   Developer Details:

    Name: Adit Shah
    ID  : 201901454
    E-mail: 201901454@daiict.ac.in or  aditshah2002@gmail.com
    College: DA-IICT

*/
/* General description of the code:

    To store customer Database and inventory database, Hash table of size 25 is used.
    (In real life, the size of this hash table can be kept larger to reduce the time complexity for search and insertion)
    The collisions have been handled by chaining, as we do not know how many customer/item will be added in total to the Database in real scenario. Hence, by using Hashing with chaining, problem of Hash tables getting overflowed is solved.

    To maintain a list of items purchased by the customer(items in cart), linked list is used.

*/
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

# define hash_table_size 25     // size of Hash table created for storing the Product as well as Customer Dataset
# define ll long long int       // defined a macro ll to get rid of writing long long int all the time

// Creation of custom data-type "item_in_stock" using structure
struct item_in_stock
{
    ll stock_item_id;
    string stock_item_name;
    float stock_item_rate;
    int stock_item_quantity;
    item_in_stock *next;
};

// Creation of custom data-type "customer" using structure
struct customer
{
    ll customer_id;
    string customer_name;
    float customer_points;
    customer *next;
};

// Creation of custom data-type "item_in_cart" to store details of each item to be purchased(i.e.kept in cart)
struct item_in_cart
{
    ll cart_item_id;
    string cart_item_name;
    float cart_item_rate;
    int cart_item_quantity;
    float cart_item_amount;
    item_in_cart *next;
};

// Hash table to store inventory stock data
    struct item_in_stock *stock_item_chain[hash_table_size]={NULL};

// Hash Table to store customer-data
    struct customer *customer_chain[hash_table_size]={NULL};

// Inventory Class
class Inventory
{
public:

    // These three functions are declared here as mentioned in the ppt but as they are accessible by Manager only, they are
    // further defined properly in Manager Class.
    virtual void Add_item(){};
    virtual void Delete_item(){};
    virtual void Update_item(){};

    /* Use of seperate update_stock() function is avoided here as the part of updating the stock is already done inside Make_Payment() function of the billing class along with generation of the bill. */

    // Function to get product info of an item
    void Get_Product_Info(ll info_id)
    {
        int has=info_id%hash_table_size;
        struct item_in_stock *tmp=stock_item_chain[has];
        int IsFound=0;           // Varible to check whether an entry was found in the Hash Table or not
        while(tmp)
        {
            // Update the item when item with given id is found in the Hash table
            if(tmp->stock_item_id==info_id)
            {
                IsFound=1;        // Changing its value to 1 as a product with given id is found in the database
                cout<<endl<<"Product-Info of Product having id: "<<info_id<<endl;
                cout<<"Product Name: "<<tmp->stock_item_name<<endl;
                cout<<"Product rate: "<<tmp->stock_item_rate<<endl;
                cout<<"Product Quantity: "<<tmp->stock_item_quantity<<endl<<endl;
            }
            tmp=tmp->next;
        }
        if(IsFound==0)      
            cout<<"OPERATION FAILED !! Can not fetch Product info as no product with given id exists."<<endl;
    }

};

// Inheriting "Manager" class from "Inventory" class ("Manager" is the child class and "Inventory" is the Parent class)
class Manager:public Inventory
{
private:
    int manager_id;
    string name;
public:
    // function to add a new item
    void Add_item(struct item_in_stock sample)
    {
        item_in_stock *np=new item_in_stock;        // allocating memory to store data of a new item to be added to stock
        np->stock_item_id=sample.stock_item_id;
        np->stock_item_name=sample.stock_item_name;
        np->stock_item_rate=sample.stock_item_rate;
        np->stock_item_quantity=sample.stock_item_quantity;
        np->next=NULL;
        int has=(np->stock_item_id)%hash_table_size;
        int isFound=0;  // 0 value of this variable indicates the item is not already present in the inventory database
        if(stock_item_chain[has]==NULL)
            stock_item_chain[has]=np;
        else            // Collisions are to be handled with Chaining(using linked list)
        {
            struct item_in_stock *tmp=stock_item_chain[has];
            while(tmp->next)
            {
                // Condition to avoid adding multiple items with same id
                if(tmp->stock_item_id==sample.stock_item_id)
                {
                    cout<<endl<<"Can not add multiple items with same id. Item with name "<<tmp->stock_item_name<<" present in inventory database also has id \""<<tmp->stock_item_id<<"\""<<endl;
                    isFound=1;
                    break;
                }
                tmp=tmp->next;
            }
            if(isFound==0)
                tmp->next=np;
        }
    }

    // function to update an item
    void Update_item(struct item_in_stock sample)
    {
        int has=sample.stock_item_id%hash_table_size;
        struct item_in_stock *tmp=stock_item_chain[has];
        int IsFound=0;           // Varible to check whether an entry was found in the Hash Table or not
        while(tmp)
        {
            // Update the item when item with given id is found in the Hash table
            if(tmp->stock_item_id==sample.stock_item_id)
            {
                tmp->stock_item_name=sample.stock_item_name;
                tmp->stock_item_rate=sample.stock_item_rate;
                tmp->stock_item_quantity=sample.stock_item_quantity;
                IsFound=1;        // Changing its value to 1 as an item with given id is found in the Database
            }
            tmp=tmp->next;
        }
        if(IsFound==0)      
            cout<<"Updating Failed !!!\n No item was found with this id. In order to update, you need to add the item first using Add_item() function "<<endl;
    }

    // function to Delete an item
    void Delete_item(ll del_id)
    {
        int has=del_id%hash_table_size;
        if(stock_item_chain[has]==NULL)
        {
            cout<<"DELETION FAILED !! No item with id "<<del_id<<" was found"<<endl;
        }
        else if(stock_item_chain[has]->stock_item_id==del_id)  // Code to delete first element of the linked list
        {
            item_in_stock *ptr;
            ptr=stock_item_chain[has];
            stock_item_chain[has]=stock_item_chain[has]->next;
            delete ptr;
        }
        else
        {
            struct item_in_stock *tmp=stock_item_chain[has];
            while(tmp)
            {
                // Declaring few pointers required for deletion
                item_in_stock *prev=tmp;
                item_in_stock *curr=tmp->next;
                if(curr==NULL)
                {
                     cout<<"DELETION FAILED !! No item with id "<<del_id<<" was found"<<endl;
                     break;
                }
                else if(curr->stock_item_id==del_id)
                {
                    prev->next=curr->next;
                    delete curr;
                    break;        // break the loop when the required element is deleted
                }
                tmp=tmp->next;
            }
        }
    }
};

// Inheriting "Billing" class from "Inventory" class ("Billing" is the child class and "Inventory" is the Parent class)
class Billing:public Inventory
{
private:
    static ll transaction_id;
    ll customer_id;
    item_in_cart *head,*tail; // all the items purchased by this customer will be stored in the form of linked_list
public:
    void set_Customer_id(ll cid)
    {
        customer_id=cid;

        // Reseting the linked list of purchased items(item in cart) to NULL for the next customer
        head=NULL;
        tail=NULL;

        // Checking if customer with this id is present in the database or not and if he is not present, he will be added to the Database by the Billing Object.
        customer *np=new customer;
        np->customer_id=cid;
        np->next=NULL;
        int has=(np->customer_id)%hash_table_size;
        int isFound=0;      // 0 value of this variable indicates customer with given id is not already present in the database and hence can be safely added as a new customer
        if(customer_chain[has]==NULL)
            customer_chain[has]=np;
        else            // When collision ocuurs
        {
            struct customer *tmp=customer_chain[has];
            while(tmp->next)
            {
                 if(tmp->customer_id==cid)
                 {
                     isFound=1;
                     break;
                 }
                 tmp=tmp->next;
            }
            // For a new customer, the biller needs to enter name of that customer too to store the entry of new customer in Database.
            if(isFound==0)
            {
                cout<<"Enter the name of the new Customer: ";
                cin>>np->customer_name;
                np->customer_points=0;
                tmp->next=np;
            }
        }

    }
    ll get_Customer_id()
    {
        return customer_id;
    }
    void set_new_Transaction_id()
    {
        transaction_id++;   // Generation of unique transaction id corresponding to each transaction
    }
    ll get_Transaction_id()
    {
        return transaction_id;
    }
    // Function to add a new customer
    void Add_customer(struct customer sample)
    {
        customer *np=new customer;
        np->customer_id=sample.customer_id;
        np->customer_name=sample.customer_name;
        np->customer_points=sample.customer_points;
        np->next=NULL;
        int has=(np->customer_id)%hash_table_size;
        int isFound=0;      // 0 value of this variable indicates customer with given id is not already present in the database and hence can be safely added
        if(customer_chain[has]==NULL)
            customer_chain[has]=np;
        else            // When collision ocuurs
        {
            struct customer *tmp=customer_chain[has];
            while(tmp->next)
            {
            // Conition to avoid adding more than one customer with the same id
                 if(tmp->customer_id==sample.customer_id)
                 {
                     isFound=1;
                     cout<<"Can not Add multiple customers with same id. Customer Name "<<tmp->customer_name<<" with same id "<<tmp->customer_id<<" is already present in the Customer Database.";
                     break;
                 }
                 tmp=tmp->next;
            }
            if(isFound==0)
                tmp->next=np;
        }
    }

    // Function to update points of a given Customer
    void Update_points(ll id_to_update,int add_points)
    {
        int has=id_to_update%hash_table_size;
        struct customer *tmp=customer_chain[has];
        int IsFound=0;           // Varible to check whether an entry was found in the Hash Table or not
        while(tmp)
        {
            // Update the item when item with given id is found in the Hash table
            if(tmp->customer_id==id_to_update)
            {
                IsFound=1;       
                tmp->customer_points=tmp->customer_points+add_points;
            }
            tmp=tmp->next;
        }
        if(IsFound==0)      
            cout<<"FATAL ERROR !! Can not update the customer points as no customer with id "<<id_to_update<<" exists"<<endl;
    }

    // Function to display information of a given Customer
    void Get_Customer_Info(ll id_to_display)
    {
        int has=id_to_display%hash_table_size;
        struct customer *tmp=customer_chain[has];
        int IsFound=0;           // Varible to check whether an entry was found in the Hash Table or not
        while(tmp)
        {
            // Update the item when item with given id is found in the Hash table
            if(tmp->customer_id==id_to_display)
            {
                IsFound=1;        // Changing its value to 1 as a customer with given input id is found
                cout<<"Customer Id: "<<tmp->customer_id<<endl;
                cout<<"Customer Name: "<<tmp->customer_name<<endl;
                cout<<"Customer Points: "<<tmp->customer_points<<endl;
            }
            tmp=tmp->next;
        }
        if(IsFound==0)      // Statement to be executed when no customer with given  id is found in the database
            cout<<"FATAL ERROR !! Could not find the customer with id "<<id_to_display<<endl;
    }

    // Function to buy a new item
    void Buy_Item(ll purchase_item_id,int quant)
    {
        int has=purchase_item_id%hash_table_size;
        struct item_in_stock *tmp=stock_item_chain[has];
        int IsFound=0;           // Varible to check whether an entry was found in the Hash Table or not
        while(tmp)
        {
            if(tmp->stock_item_id==purchase_item_id)
            {
                item_in_cart *np=new item_in_cart;
                np->cart_item_id=purchase_item_id;
                np->cart_item_name=tmp->stock_item_name;
                np->cart_item_rate=tmp->stock_item_rate;
                np->cart_item_quantity=quant;
                //np->cart_item_amount=(np->cart_item_quantity)*(np->cart_item_rate);
                np->next=NULL;
                if(head==NULL)
                {
                    head=np;
                    tail=np;
                }
                else
                {
                    tail->next=np;
                    tail=np;
                }
                IsFound=1;        // Changing its value to 1 as an item with given input id is found
            }
            tmp=tmp->next;
        }
        if(IsFound==0)      // Statement to be executed when no item with given id is found
           cout<<"SORRY !! The item with id "<<purchase_item_id<< " is currently out of stock... "<<endl;
    }

    // Equivalent to deleting a node from linked list, we will delete the item with given id from linked list of type item_in_cart in which all data of purchased items is stored
    void Cancel_Item(ll cancel_item_id)
    {
        if(head==NULL)
        {
            cout<<"CANCELLATION FAILED !! You have not purchased any item yet "<<endl;
        }
        else if(head->cart_item_id==cancel_item_id)  // Code to delete first element of the linked list
        {
            item_in_cart *ptr;
            ptr=head;
            head=head->next;
            delete ptr;
        }
        else
        {
            struct item_in_cart *dp=head;
            while(dp)
            {
                item_in_cart *prev=dp;
                item_in_cart *curr=dp->next;
                if(curr==NULL)
                {
                    cout<<"CANCELLATION FAILED !! You have not purchased any item with id "<<cancel_item_id<<endl;
                    break;
                }
                if(curr->cart_item_id==cancel_item_id)
                {
                    prev->next=curr->next;
                    delete curr;
                    break;
                }
                dp=dp->next;
            }
        }
    }

    void Make_Payment()
    {
        // Print transaction id and Customer id on the top of the bill
        cout<<endl<<"-------Your bill for today's purchase-------"<<endl<<endl;
        cout<<"Transaction Id : "<<transaction_id<<endl;
        cout<<"Customer Id    : "<<customer_id<<endl;
        cout<<"Product ID \t"<<"Name \t"<<"Rate \t"<<"Quantity \t"<<"Amount"<<endl;
        item_in_cart *mp;
        float tot_amount=0;     // Intitializing the value of total amount to 0 for a new bill
        mp = head;
        while (mp != NULL)
        {
            //Update the quantity of stock in Inventory Database after the purchase: task of update_stock() function
            int has=(mp->cart_item_id)%hash_table_size;
            struct item_in_stock *tmp=stock_item_chain[has];
            while(tmp)
            {
                if(tmp->stock_item_id==mp->cart_item_id)
                {
                
/*If the quantity of any item mentioned by billing object is more than that present in the stock, he will be given a message on the console and his bill will be calculated only for the number of pieces present in the stock. Thereby after the purchase, pieces of that item in the stock will become 0.*/
                    if((mp->cart_item_quantity)>(tmp->stock_item_quantity))
                    {
                        cout<<endl<<"SORRY !! Only "<<tmp->stock_item_quantity<<" pieces are left for item with id "<<mp->cart_item_id<<" in stock."<<endl;
                        cout<<"So instead of "<<mp->cart_item_quantity<<" pieces, Your bill will be made only for "<<tmp->stock_item_quantity<<" pieces."<<endl<<endl;
                        mp->cart_item_quantity=tmp->stock_item_quantity;        // As the Ordered pieces exceed the pieces in stock, ordered pieces will become same as the pieces in stock
                        tmp->stock_item_quantity=0;    // Make the number of pieces in original stock nil
                    }
                    else
                        tmp->stock_item_quantity=(tmp->stock_item_quantity)-(mp->cart_item_quantity);   // update the stock
                }
                tmp=tmp->next;
            }
            // calculating the amount corresponding to that item
            mp->cart_item_amount=(mp->cart_item_rate)*(mp->cart_item_quantity);
            cout <<mp->cart_item_id<<"\t"<<mp->cart_item_name<<"\t"<<mp->cart_item_rate<<"\t"<<mp->cart_item_quantity<<"\t\t"<<mp->cart_item_amount<<endl;
            // Add to total amount
            tot_amount=tot_amount+(mp->cart_item_amount);
            mp = mp->next;
        }
        Update_points(customer_id,tot_amount);      // to update points of the customer after the purchase
        cout<<"The total amount to be paid is:  "<<tot_amount<<endl;
    }
};
ll Billing::transaction_id=8790427391;   // Initializing the static variable transaction_id to some random value

// Main Function
int main()
{
        cout<<"----------WELCOME TO ADIT'S(201901454) SUPER MARKET SOFTWARE-----------------"<<endl<<endl;
    
    // Loading the Inventory Dataset
        struct item_in_stock INVENTORY_DATASET[100] = {
        {111100000001,"1",100,20},{111100000002,"2",110,20},{111100000003,"3",120,20},{111100000004,"4",130,20},{111100000005,"5",140,20},{111100000006,"6",150,20},{111100000007,"7",160,20},{111100000008,"8",170,20},{111100000009,"9",180,20},{111100000010,"10",190,20},
        {111100000011,"11",200,20},{111100000012,"12",210,20},{111100000013,"13",220,20},{111100000014,"14",230,20},{111100000015,"15",240,20},{111100000016,"16",250,20},{111100000017,"17",260,20},{111100000018,"18",270,20},{111100000019,"19",280,20},{111100000020,"20",290,20},
        {111100000021,"21",300,20},{111100000022,"22",310,20},{111100000023,"23",320,20},{111100000024,"24",330,20},{111100000025,"25",340,20},{111100000026,"26",350,20},{111100000027,"27",360,20},{111100000028,"28",370,20},{111100000029,"29",380,20},{111100000030,"30",390,20},
        {111100000031,"31",400,20},{111100000032,"32",410,20},{111100000033,"33",420,20},{111100000034,"34",430,20},{111100000035,"35",440,20},{111100000036,"36",450,20},{111100000037,"37",460,20},{111100000038,"38",470,20},{111100000039,"39",480,20},{111100000040,"40",490,20},
        {111100000041,"41",500,20},{111100000042,"42",510,20},{111100000043,"43",520,20},{111100000044,"44",530,20},{111100000045,"45",540,20},{111100000046,"46",550,20},{111100000047,"47",560,20},{111100000048,"48",570,20},{111100000049,"49",580,20},{222200001111,"50",590,20},
        {222200001114,"51",600,20},{222200001117,"52",610,20},{222200001120,"53",620,20},{222200001123,"54",630,20},{222200001126,"55",640,20},{222200001129,"56",650,20},{222200001132,"57",660,20},{222200001135,"58",670,20},{222200001138,"59",680,20},{222200001141,"60",690,20},
        {222200001144,"61",700,20},{222200001147,"62",710,20},{222200001150,"63",720,20},{222200001153,"64",730,20},{222200001156,"65",740,20},{222200001159,"66",750,20},{222200001162,"67",760,20},{222200001165,"68",770,20},{222200001168,"69",780,20},{222200001171,"70",790,20},
        {222200001174,"71",800,20},{222200001177,"72",810,20},{222200001180,"73",820,20},{222200001183,"74",830,20},{222200001186,"75",840,20},{222200001189,"76",850,20},{222200001192,"77",860,20},{222200001195,"78",870,20},{222200001198,"79",880,20},{222200001201,"80",890,20},
        {222200001204,"81",900,20},{222200001207,"82",910,20},{222200001210,"83",920,20},{222200001213,"84",930,20},{222200001216,"85",940,20},{222200001219,"86",950,20},{222200001222,"87",960,20},{222200001225,"88",970,20},{222200001228,"89",980,20},{222200001231,"90",990,20},
        {222200001234,"91",1000,20},{222200001237,"92",1010,20},{222200001240,"93",1020,20},{222200001243,"94",1030,20},{222200001246,"95",1040,20},{222200001249,"96",1050,20},{222200001252,"97",1060,20},{222200001255,"98",1070,20},{222200001258,"99",1080,20},{222200001261,"100",1090,20},
        };

        // Loading the Customer Dataset
        struct customer CUSTOMER_DATASET[100] = {
        {9400000001,"1",0},{9400000002,"2",0},{9400000003,"3",0},{9400000004,"4",0},{9400000005,"5",0},{9400000006,"6",0},{9400000007,"7",0},{9400000008,"8",0},{9400000009,"9",0},{9400000010,"10",0},
        {9400000011,"11",0},{9400000012,"12",0},{9400000013,"13",0},{9400000014,"14",0},{9400000015,"15",0},{9400000016,"16",0},{9400000017,"17",0},{9400000018,"18",0},{9400000019,"19",0},{9400000020,"20",0},
        {9400000021,"21",0},{9400000022,"22",0},{9400000023,"23",0},{9400000024,"24",0},{9400000025,"25",0},{9400000026,"26",0},{9400000027,"27",0},{9400000028,"28",0},{9400000029,"29",0},{9400000030,"30",0},
        {9400000031,"31",0},{9400000032,"32",0},{9400000033,"33",0},{9400000034,"34",0},{9400000035,"35",0},{9400000036,"36",0},{9400000037,"37",0},{9400000038,"38",0},{9400000039,"39",0},{9400000040,"40",0},
        {9400000041,"41",0},{9400000042,"42",0},{9400000043,"43",0},{9400000044,"44",0},{9400000045,"45",0},{9400000046,"46",0},{9400000047,"47",0},{9400000048,"48",0},{9400000049,"49",0},{9400000050,"50",0},
        {9400000051,"51",0},{9400000052,"52",0},{9400000053,"53",0},{9400000054,"54",0},{9400000055,"55",0},{9400000056,"56",0},{9400000057,"57",0},{9400000058,"58",0},{9400000059,"59",0},{9400000060,"60",0},
        {9400000061,"61",0},{9400000062,"62",0},{9400000063,"63",0},{9400000064,"64",0},{9400000065,"65",0},{9400000066,"66",0},{9400000067,"67",0},{9400000068,"68",0},{9400000069,"69",0},{9400000070,"70",0},
        {9400000071,"71",0},{9400000072,"72",0},{9400000073,"73",0},{9400000074,"74",0},{9400000075,"75",0},{9400000076,"76",0},{9400000077,"77",0},{9400000078,"78",0},{9400000079,"79",0},{9400000080,"80",0},
        {9400000081,"81",0},{9400000082,"82",0},{9400000083,"83",0},{9400000084,"84",0},{9400000085,"85",0},{9400000086,"86",0},{9400000087,"87",0},{9400000088,"88",0},{9400000089,"89",0},{9400000090,"90",0},
        {9400000091,"91",0},{9400000092,"92",0},{9400000093,"93",0},{9400000094,"94",0},{9400000095,"95",0},{9400000096,"96",0},{9400000097,"97",0},{9400000098,"98",0},{9400000099,"99",0},{9400000100,"100",0},
        };

    Manager i_am_manager;       // Declaring object of "Manager" class
    Billing i_am_biller;        // Declaring object of "Billing" class
    
    // Incorporating the inventory and Customer Dataset that we loaded just now
    for(int i=0;i<100;i++)
    {
        // Manager object reads item dataset and uploads to inventory database 
        i_am_manager.Add_item(INVENTORY_DATASET[i]);

        //Billing object reads provided customer data and uploads to customer database
        i_am_biller.Add_customer(CUSTOMER_DATASET[i]);
    }
    
    //Part to test delete_item() and update_item()
    i_am_manager.Add_item({222200001264,"101",2000,20});
    i_am_manager.Update_item({222200001264,"101",2010,20});
    // i_am_biller.Delete_item(222200001264); will give an error as Delete_item is accessible by manager objects only and not by billing objects
    i_am_manager.Delete_item(222200001264);
    
    //Billing Object now generates a shopping episode with a customer

    i_am_biller.set_Customer_id(9400000001);
    // If customer with the id entered above is not already present in the database, it will prompt to ask
    // the name of this new customer and thereafter it will add this customer in the Customer Database.
    
    cout<<endl<<"------------------------------------------------------"<<endl;
    
    // Displaying Customer points of this customer before the purchase which should be 0
    cout<<"Customer points before the purchase: "<<endl<<endl;
    i_am_biller.Get_Customer_Info(9400000001);
    
    cout<<endl<<"------------------------------------------------------"<<endl;
    
    // This customer buys following three ITEMS
    i_am_biller.Buy_Item(111100000011,3);   // ITEM 1 with quantity 3
    i_am_biller.Buy_Item(222200001114,1);   // ITEM 2 with quantity 1
    i_am_biller.Buy_Item(222200001234,2);   // ITEM 3 with quantity 2
    
    // To test cancel_item function
    i_am_biller.Buy_Item(222200001255,5);     
    i_am_biller.Cancel_Item(222200001255);   
    
    // Biller Object will set new transaction id,make payment and print the bill
    i_am_biller.set_new_Transaction_id();
    i_am_biller.Make_Payment();
     
    cout<<endl<<"------------------------------------------------------"<<endl;
    
    // The points of this customer have got updated now 
    cout<<endl<<"Customer points after the purchase: "<<endl<<endl;
    i_am_biller.Get_Customer_Info(9400000001);
    
    cout<<endl<<"------------------------------------------------------"<<endl;
    
    //Manager object prints information of the items purchased to observe the decrease in stock-quantity of those purchased items
    i_am_manager.Get_Product_Info(111100000011);
    i_am_manager.Get_Product_Info(222200001114);
    i_am_manager.Get_Product_Info(222200001234);
    
    return 0;
}
