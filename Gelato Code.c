#include<stdio.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
void use();
void checkStatus();
void customer();
void removeItem(int ic);
struct node
{
	struct node *prev;
	struct node *next;
	int code;
	int quan;
	double time;
	int ono;	
};
typedef struct node order;
order *front=NULL, *rear=NULL, *new, *temp, *prev, *next;
int orderstaken=0,orderno=0,rem=0;
char passwd[]="vanshika";
clock_t begin, end;
double time_spent;
FILE *f1,*f2,*f3,*fp1,*fp2,*fp3;

void insertOrder(int ic,int q,int orderno,int t)
{
	//insert order in the end of circular doubly linked list
	if(front==NULL)
	{
		new=(order*)malloc(sizeof(order));
		new->code=ic;
		new->quan=q;
		new->ono=orderno;
		new->time=t;
		new->prev=NULL;
		new->next=NULL;
		front=new;
		rear=new;
		new->next=front;
		front->prev=new;
	}
	else
	{
		new=(order*)malloc(sizeof(order));
		new->code=ic;
		new->quan=q;
		new->ono=orderno;
		new->time=t;
		new->next=NULL;
		new->prev=front->prev;
		new->next=front;
		front->prev=new;
		rear->next=new;
		rear=new;
	}
}
void deleteOrder()
{
	//delete order in the beginning of circular doubly linked list
	if(front==NULL)
	{
		printf("All orders are served");
	}
	else
	{
		if(front==rear)
	    {
	     	temp=front;
	    	front=rear=NULL;
    	}
	    else
	    {
		    temp=front;
	        front=front->next;
	        front->prev=temp->prev;
	        rear->next=front;
	    }
        printf("\nOrder no. %d is READY!!\n",temp->ono);
	    printf("Item code: %d\n",temp->code);
	    printf("Quantity: %d\n",temp->quan);
	    free(temp);
	    orderno--;
	}	
}
void displayQueueStatus()
{
  //display orders in the circular doubly linked list
  printf("\nQUEUE STATUS\n------------\n");
  int i=0;
  if(front==NULL)
     printf("NO ORDERS LEFT IN THE QUEUE\n");
  else
  {
  	temp=front;
  	do
  	{
  	 printf("%d:: Order No. %d | Item Code: %d | Quantity: %d\n",i+1,temp->ono,temp->code,temp->quan);
	 temp=temp->next;
	 i++;	
	}while(temp!=rear->next);
	printf("|\n|\nV\nEND OF ORDER QUEUE\n\n");
  }
  end=clock();
  if(orderno!=0)
    checkStatus();	
}
void printMenu()//print ice cream menu
{
  char ch, temp,pr[3],item[101];
  int i;
  f1=fopen("itemname.txt","r");//This file has item code and name
  f2=fopen("price.txt","r");//This file has price of every item
  if(f1==NULL || f2==NULL)
  {
  	printf("Error in opening file\n");
  	exit(0);
  }
  printf("ITEM CODE   ITEM NAME\t\t\t\t\t\t\t\t\t\t    PRICE\n");
  while((ch=fgetc(f1))!=EOF)
  {
	if(ch=='_')
	{
	  fscanf(f2, "%s", pr);	
	  printf("Rs. %s\n",pr); 	
	  printf("----------\n");
	}
    if(ch=='-')
	{
	 for(i=0;i<101;i++)
	    item[i]=' ';
	 item[100]='\0';
	 i=0;
  	 while((temp=fgetc(f1))!='_')
  	  {
  	  	item[i]=temp;
  	  	i++;
	  }
	  
	  printf("%s",item);
	}
  } 
  fclose(f1);
  fclose(f2);	
}
void checkStatus()
{
	//to check if the order is processed
	char ch;
	double sumtime;
	int i,j;
	while(ch!='C')
	{
	  printf("Press C to continue...\n");
	  scanf(" %c",&ch);	
	  if(ch!='C')
	     printf("Please enter a valid choice\n\n");
	}
	time_spent=(double)(end-begin)/CLOCKS_PER_SEC;
	sumtime=0;
	if(time_spent==front->time)
	    deleteOrder();
	else if(time_spent>front->time)
	{
		temp=front;
		for(i=1;(time_spent>sumtime)&&(temp!=rear);i++)
		{
			sumtime+=temp->time;
			temp=temp->next;
		}
		if(time_spent>=sumtime)
		{
			if(i==1)
			{
			   deleteOrder();
			}
			else
			{
			   for(j=1;j<i;j++)
			   {
			     deleteOrder();
			   }
			   front->time-=(time_spent-sumtime);
		    }
		}
		else if(time_spent<sumtime)
		{
			for(j=1;j<(i-1);j++)
			{
				deleteOrder();
			}
			front->time=sumtime-time_spent;
		}
	}
	else
	{
		front->time=front->time-time_spent;
	}
	
}

void checkOut(int ic,int quantity)
{
  /*traverse through the file with item code
     and find the price of that item*/
  int count=1,bursttime_min,bursttime_sec,p,temp,line;
  float bill;
  char c,parcel='3';
  f1=fopen("itemname.txt","r");
  f2=fopen("price.txt","r");
  if(f1==NULL || f2==NULL)
  {
  	printf("Error in opening file\n");
  	exit(0);
  }
  c=getc(f1);
  while(c != EOF) 
  {
		if(c=='-')
        {
        	fscanf(f1,"%d",&temp);
		}
        if (c == '\n')
        {
        	if(temp==ic)
        	{
        		line=count;
			}
			count++;	
		}
        c = getc(f1);
  }
  count=1;
  while ((fscanf(f2,"%d",&p))!= EOF) 
  {  
    if (count == line)
    {   
      bill=quantity*p; 
      break;
	}   
    else
      count++;   
  }   
  fclose(f1);
  fclose(f2);
  if(rem==1)
       removeItem(ic);
  while(parcel!='1'&&parcel!='2') 
  {
  	//To calculate parcel amount
  	printf("\n\nIs your order:\n1)Take away\n2)Eat here\nPlease enter your choice:");
    scanf(" %c",&parcel);
    if(parcel!='1'&&parcel!='2')
    {
      	 printf("Please enter a valid choice\n\n");
	}	
  }
  if(parcel=='1')
    bill+=10; 
  orderstaken++;
  orderno++;
  printf("Enter the time (in minutes) required to complete this order:");
  scanf("%d",&bursttime_min);
  bursttime_sec=bursttime_min*60;
  printf("\nYOUR ORDER DETAILS ARE:\n");
  printf("Order No. : %d\nItem code: %d\n",orderstaken,ic);
  printf("Quantity: %d\nTOTAL AMOUNT: %.2f\n",quantity,bill);
  printf("Please pay the bill...\n\n");
  printf("Thanks for visiting!\n\nPlease wait you are in queue...\n\n");
  //adding icode, qty, order no,burst time to linked list	
  insertOrder(ic,quantity,orderstaken,bursttime_sec); 
  end=clock();
  if(orderno!=1)
     checkStatus();
}
void takeOrder()
{
  int icode, qty,user_choice;
  printf("Enter Itemcode: ");
  scanf("%d",&icode);
  printf("Enter Quantity: ");
  scanf("%d",&qty);
  user_choice=checkQuantity(icode,qty);//check for quantity
  if(user_choice==0)
     customer();
  else
     checkOut(icode,user_choice);   
}
void budget(int b)
{
	/*this function filters out the items based on 
	 the budget of the customer*/
	int count=0,i=0,temp,arr[50],flag=0;
	char ch,item[101],pr[3];
	f1=fopen("itemname.txt","r");
	f2=fopen("price.txt","r");
	if(f1==NULL || f2==NULL)
    {
  	printf("Error in opening file\n");
   	exit(0);
    }
 	for(i=0;i<50;i++)
   	{
   	    arr[i]=0;
	}
	i=0;
	while ((fscanf(f2,"%d",&temp))!= EOF)
    {
        count++;
        if (temp<=b)
        {
            arr[i]=count;
            i++;
        }
    }
    count=1;
    rewind(f2);
    if(i==0)
    {
    	printf("NO ITEMS WITHIN BUDGET Rs. %d\n\n",b);
	}
	else
	{
    printf("\n\nTHESE ARE YOUR RECOMMENDATIONS:\n");
    printf("ITEM CODE   ITEM NAME\t\t\t\t\t\t\t\t\t\t    PRICE\n");
    while((ch=fgetc(f1))!=EOF)
   {
   	for(i=0;i<50;i++)
   	{
   	    if(count==arr[i])
		   flag=1;	
	}
	if(ch=='_')
	{
	   fscanf(f2, "%s", pr);
	   if(flag==1)
	  {	
	   printf("Rs. %s\n",pr); 	
	   printf("----------\n");
      }
      count++;
      flag=0;
	}
    if(ch=='-')
	{
	 for(i=0;i<101;i++)
	    item[i]=' ';
	 item[100]='\0';
	 i=0;
  	 while((temp=fgetc(f1))!='_')
  	  {
  	  	item[i]=temp;
  	  	i++;
	  }
	  if(flag==1)
	     printf("%s",item);
	}
  }
 }
  fclose(f1);
  fclose(f2); 
    
}
void customer()
{
	char proceed='y';
	int bud;
    printf("\n\nWelcome to GELATO-The Ice Cream Parlour!\n\n");
    printf("TODAY'S MENU\n");
    printf("______________\n\n");
	printMenu(); //Print menu with item name, item code, and price
	printf("\n");
	while(proceed!='Y'&&proceed!='N')
	{
	   printf("\nDo you want to search for items below a price? (Y/N) ");
	    scanf(" %c",&proceed);
	   if(proceed!='Y'&&proceed!='N')
       {
      	  printf("Please enter a valid choice\n\n");
	   }		
	}
    if(proceed=='Y')
    {
    	printf("Enter the maximum price: ");
    	scanf("%d",&bud);
		budget(bud);//search according to budget
	}
	proceed='y';
	while(proceed!='Y'&&proceed!='N')
	{
	  printf("\nWould you like to proceed with the order? ");
	  printf("[Enter Y if you want to proceed else enter N]\n");
	  scanf(" %c",&proceed);
	  if(proceed!='Y'&&proceed!='N')
      {
      	  printf("Please enter a valid choice\n\n");
	  }	
	}
	if(proceed=='Y')
	{ 
	   printf("\nYOUR ORDER PLEASE...\n\n");
	   takeOrder();
	   use();
	}
	else
	{
		printf("No problem...\nThanks for visiting!\n");
		printf("Please do visit us again. Have a great day!\n");
		end=clock();
		if(orderno!=0)
		   checkStatus();
		use();
	}	
}
void addItem()
{
	char newitem[100],c='-',p[4],dummy;
	char space1[]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\0'};
	char space2[]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\0'};
	char ch1[]={'_','_','\n','\0'};
	int new_price,quantity,itemc;
	printf("Enter the new item code: ");
	scanf("%d",&itemc);
	scanf("%c",&dummy);
	printf("Enter the new item name: ");
	scanf("%[^\n]",newitem);
	newitem[100]='\0';
	printf("Enter the price of the item: ");
	scanf("%d",&new_price);
	printf("Enter the quantity of item no.%d: ",itemc);
	scanf("%d",&quantity);
	f1=fopen("itemname.txt","a");
	f2=fopen("price.txt","a");
	fp1=fopen("quantity.txt","a");
	if(f1==NULL || f2==NULL ||fp1==NULL)
    {
  	printf("Error in opening file\n");
  	exit(0);
    }
   fprintf(f2,"%d\n",new_price);
   fprintf(fp1,"%d\n",quantity);
   fprintf(f1,"%c",c);
   fprintf(f1,"%d",itemc);
   if(itemc>=9)
      fputs(space2,f1);
   else
      fputs(space1,f1);
   fputs(newitem,f1);
   fputs(ch1,f1);
   fclose(f1);
   fclose(f2);
   fclose(fp1);
   end=clock();
   if(orderno!=0)
		   checkStatus();
   use();
}
void removeItem(int ic)
{
	char c,p;
    int temp,count=1,line=0;
	f1=fopen("itemname.txt","r");
	f2=fopen("price.txt","r");
	f3=fopen("quantity.txt","r");
    fp1=fopen("dummyic.txt","w");
    fp2=fopen("dummyp.txt","w");
    fp3=fopen("dummyq.txt","w");
    if(f1==NULL||f2==NULL||f3==NULL||fp1==NULL||fp2==NULL||fp3==NULL)
    {
    	printf("Error in opening file\n");
    	exit(0);
    }
    c=getc(f1);
    while (c != EOF) 
	{
		if(c=='-')
        {
        	fscanf(f1,"%d",&temp);
        	if(temp!=ic)
        	{
        		putc(c,fp1);
        		fprintf(fp1,"%d",temp);
			}
		}
        else if (temp!=ic)
        {
           putc(c, fp1);
        }
        if (c == '\n')
        {
        	if(temp==ic)
        	{
        		line=count;
			}
			count++;	
		}
        c = getc(f1);
    }
    if(line==0)
       printf("Item not found\n");
    count=1;
    p=getc(f2);
    while (p!= EOF) 
	{
        if (count!=line)
        {
           putc(p, fp2);
        }
        if (p == '\n')
           count++;
        p=getc(f2);
    }
    count=1;
    p=getc(f3);
    while (p!= EOF) 
	{
        if (count!=line)
        {
           putc(p, fp3);
        }
        if (p == '\n')
           count++;
        p=getc(f3);
    }
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(f1);
    fclose(f2);
    fclose(f3);
    remove("itemname.txt");
    remove("price.txt");
    remove("quantity.txt");
    rename("dummyic.txt","itemname.txt");
    rename("dummyp.txt","price.txt");
    rename("dummyq.txt","quantity.txt");
}
int checkQuantity(int ic,int qu)
{
  /*checks if the item required is available in stock*/
  int count=1,temp,line=0,flag,quan;
  char c,ch;
  f1=fopen("quantity.txt","r");
  f2=fopen("itemname.txt","r");
  fp1=fopen("dummyq.txt","w");
  if(f1==NULL||f2==NULL||fp1==NULL)
  {
  	printf("Error in opening file\n");
  	exit(0);
  }
  c=getc(f2);
  while(c != EOF) 
  {
		if(c=='-')
        {
        	fscanf(f2,"%d",&temp);
		}
        if (c == '\n')
        {
        	if(temp==ic)
        	{
        		line=count;
			}
			count++;	
		}
        c = getc(f2);
  }
    count = 0;
    while ((fscanf(f1,"%d",&temp))!= EOF)
    {
        count++;
        if (count == line)
        {
        	quan=temp;
        	if(qu<quan)
            {
            	quan=quan-qu;
            	flag=qu;
            	fprintf(fp1,"%d\n",quan);
			}
			else if(qu==quan)
			{
				flag=quan;
				fprintf(fp1,"%d\n",quan);
				rem=1;
			}
			else
			{
				printf("Only %d ice creams are available\n",quan);
				printf("Do you want to continue (Y/N): ");
				scanf(" %c",&ch);
				if(ch=='Y')
				{
					flag=quan;
					fprintf(fp1,"%d\n",quan);
					rem=1;
				}
				else
				{
				    fprintf(fp1,"%d\n",quan);
				    flag=0;
				}
			}
		}
		else
		   fprintf(fp1,"%d\n",temp);

    }
    fclose(fp1);
    fclose(f1);
    fclose(f2);
    remove("quantity.txt");
    rename("dummyq.txt","quantity.txt");
    return flag;
}
void changePrice()
{
    char c,p;
    int temp,ic,count=1,line=0,newprice;
	f1=fopen("itemname.txt","r");
	f2=fopen("price.txt","r");
	if(f1==NULL || f2==NULL)
    {
    	printf("Error in opening file\n");
    	exit(0);
    }
	printf("\nEnter item code: ");
    scanf("%d", &ic);
    printf("\nEnter the new price of item %d: ",ic);
    scanf("%d",&newprice);
    fp2=fopen("dummyp.txt","w");
    c=getc(f1);
    while (c != EOF) 
	{
		if(c=='-')
        {
        	fscanf(f1,"%d",&temp);
		}
        if (c == '\n')
        {
			if(temp==ic)
        	{
        		line=count;
			}
			count++;	
		}
        c = getc(f1);
    }
    if(line==0)
       printf("Item not found\n");
    count = 0;
    while ((fscanf(f2,"%d",&temp))!= EOF)
    {
        count++;
        if (count == line)
            fprintf(fp2,"%d\n",newprice);
        else
            fprintf(fp2,"%d\n",temp);
    }
    fclose(fp2);
    fclose(f1);
    fclose(f2);
    remove("price.txt");
    rename("dummyp.txt","price.txt");
    end=clock();
    if(orderno!=0)
		   checkStatus();
    use();
}
void employee()
{
    char c='5';
    int item_code;
    while(c!='1'&&c!='2'&&c!='3'&&c!='4')
	{
	  printf("\n1)Add a new item\n2)Remove an item\n3)Change the price of an item\n");
	  printf("4)Print menu\nEnter your choice: ");
      scanf(" %c",&c);
      if(c!='1'&&c!='2'&&c!='3'&&c!='4')
	  {
	     printf("Please enter a valid choice\n\n");
	  }	
	}
	switch(c)
    {
    	case '1':addItem();
    	         break;
        case '2':printf("\nEnter item code to be deleted: ");
                 scanf("%d", &item_code);
		         removeItem(item_code);
		         end=clock();
                 if(orderno!=0)
  		            checkStatus();
                 use();
    	         break;
        case '3':changePrice();
                 break;
        case '4':printMenu();
                 end=clock();
                 if(orderno!=0)
	         	   checkStatus();
                 use();
                 break;
	}
}
int password()
{
	/*password for employee access is asked
	  and it is masked to prevent others from seeing it*/
	char password[10],ch;
    int i, j;
    printf("Enter the password : ");
    for(i = 0;i < 20;i++) 
   {
      ch = getch();
      if(ch == 13)
         break; 
      password[i] = ch;
      ch = '*';
      printf("%c", ch);
   }
   password[i] = '\0';
   if(strcmp(password,passwd)==0)
   {
	   return 1;
   }
   else
   {
   	 printf("\nINVALID PASSWORD\n");
   	 end=clock();
   	 return 0;
   }
}
void use()
{
	char ch='5';
	int permission;
	begin=clock();
	while(ch!='1'&&ch!='2'&&ch!='3'&&ch!='4')
	{
	  printf("\n1)Take customer's order\n2)Employee usage\n3)Check queue status\n");
	  printf("4)Shut down\nEnter your choice: ");
      scanf(" %c",&ch);
      if(ch!='1'&&ch!='2'&&ch!='3'&&ch!='4')
	  {
	     printf("Please enter a valid choice\n\n");
	  }	
	}
    switch(ch)
    {
    	case '1':customer();
    	         break;
        case '2':permission=password();
		         if(permission==1)
		             employee();
		         else
		         {
		         	if(orderno!=0)
		         	   checkStatus();
		            use();
		         }
    	         break;
        case '3':displayQueueStatus();
                 use();
                 break;
        case '4':exit(0);
                 break;
	}
}
void main()
{
	printf("WELCOME TO GELATO!\nHOPE YOU HAVE A GREAT DAY!\n");
	use();
}


