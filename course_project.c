#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define row 7
#define col 10
#define ticketamount 200
#define snacksamount 100

int seats[row][col]={0};

int get_rows() { return row; }
int get_cols() { return col; }

typedef struct BookingDetails{
 char name[20];
 int SeatNo;
 float Amtpaid;
 char snacks;
 char date[20];
 char time[20];
} BookingDetails;

// Define Node
typedef struct Node {
    BookingDetails data;
    struct Node* next;
} Node;

Node* head=NULL;
Node* tail=NULL;

// Function to create a new Node
Node* createNode(BookingDetails b) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->data = b;
    n->next = NULL;
    return n;
}

void insertBooking(BookingDetails b){
    Node*newNode=createNode(b);
    if(head==NULL) {
        head=newNode;
        tail=newNode;
    } else {
        Node*temp=head;
        /*while (temp->next!=NULL) {
            temp=temp->next;
        }
        temp->next=newNode;*/
        tail->next=newNode;
        tail=tail->next;
    }
}
int updatebooking(int R,int C,char snacks)
{
    int seatno;
    Node* temp=head;
    seatno=((R+1)*col+(C+1))-10;
    printf("\n inside update booking seat no is %d\n",seatno);
    while(temp!=NULL)
    {
        if(temp->data.SeatNo==seatno)
        {
            if(temp->data.snacks==snacks)
            {
                printf("\n Booking already exists. No change");
                return 2;
            }
            else
            {
                char originalsnack=temp->data.snacks;
                temp->data.snacks=snacks;
                if(originalsnack=='Y')
                {
                    temp->data.Amtpaid=temp->data.Amtpaid-snacksamount;
                    printf("\n Booking is updated from snacks required to snacks not required \n");
                    printf("Date:%s|Time:%s|Amount Paid:%.2f\n",temp->data.date,temp->data.time,temp->data.Amtpaid);
                    return 3;

                }
                else
                {
                    temp->data.Amtpaid=temp->data.Amtpaid+snacksamount;
                    printf("\n Booking is updated from snacks not required to snacks required \n");
                    printf("Date:%s|Time:%s|Amount Paid:%.2f\n",temp->data.date,temp->data.time,temp->data.Amtpaid);
                    return 4;
                }

            }
        }
        temp=temp->next;

    }

}
int bookseat(int R, int C,char name[], char snacks){

    R=R-1;
    C=C-1;
    if(R<=-1||R>=row||C<=-1||C>=col) {
        printf("Invalid seat position!\n");
        return 1;
    }

    //printf("\n doubt here %d\n ",seats[R][C]);
    if(seats[R][C]==0)
    {
        //printf("\n doubt here inside if %d\n ",seats[R][C]);
        seats[R][C]=1;

        BookingDetails b;
        strcpy(b.name,name);
        b.snacks=snacks;
        printf("(R+1)=%d \t (C+1)=%d \t col=%d \t seatno=%d \t",(R+1),(C+1),col,(R+1)*col+(C+1)-10);
        //getchar();
        b.SeatNo=((R+1)*col+(C+1))-10;
        if(snacks=='Y')
        {
            b.Amtpaid = ticketamount+snacksamount;
        }
        else
        {
            b.Amtpaid=ticketamount;
        }

        time_t t;
        struct tm *tm_info;
        time(&t);
        tm_info = localtime(&t);
        strftime(b.date, sizeof(b.date), "%d-%m-%Y", tm_info);
        strftime(b.time, sizeof(b.time), "%H:%M:%S", tm_info);

        insertBooking(b);
        printf("Seat (%d) Booked for %s!\n",b.SeatNo,name);
        printf("Date:%s|Time:%s|Amount Paid:%.2f\n",b.date,b.time,b.Amtpaid);
    }
    else
    {
        int result=updatebooking(R,C,snacks);
        printf("The Seat is already booked");
        return result;
    }
    return 0;
}

int cancelseat(int R, int C)
{
    R -= 1;
    C -= 1;

    if (R < 0 || R >= row || C < 0 || C >= col) {
        printf("Invalid seat position!\n");
        return 1;
    }

    printf("\n inside cancelseat\n");
    int seatNo = (R * col) + (C + 1);
    printf("\n before checking if head is null seatno=%d", seatNo);
    if(head==NULL)
    {
        printf("\n Booking not found in history!\n");
        return 2;
    }

    Node *temp = head;

    if(head->data.SeatNo==seatNo)
    {
        temp=head;
        //free(temp);
        seats[R][C] = 0;
        printf("\nSeat (%d,%d) reservation canceled successfully!\n", R + 1, C + 1);
        temp=head;
        head=head->next;
        free(temp);
        return 0;
    }
    printf("\n inside cancel before while \n");
    while(temp->next !=NULL && temp->next->data.SeatNo!= seatNo)
    {
        temp=temp->next;
    }
    if (temp->next == NULL) {
        printf("Booking not found in history!\n");
        return 2;
    }
    else
    {
        Node* temp1=temp->next;
        temp->next=temp1->next;
        free(temp1);
        seats[R][C] = 0;
        printf("Seat (%d,%d) reservation canceled successfully!\n", R + 1, C + 1);
        return 0;

    }



}


char* bookhistory() {
    if(head == NULL) {
        char* empty = (char*)malloc(50);
        strcpy(empty, "No bookings yet!");
        return empty;
    }

    // estimate buffer size
    char* buffer = (char*)malloc(5000);
    buffer[0] = '\0';

    Node* temp = head;
    char line[200];
    char line1[200];
    char line2[200];
    snprintf(line1,sizeof(line1),"\n---------------- Booking History ----------------\n");
    strcat(buffer, line1);
    while(temp != NULL)
        {
            snprintf(line, sizeof(line),
            "Name:%s\nSeatNo:%d(Row:%d;Col:%d)\nAmount:%.2f\nDate:%s \nTime:%s\nSnacks:%c\n",
            temp->data.name,
            temp->data.SeatNo,
            ((temp->data.SeatNo / col) +1== 8) ? 7 : (((temp->data.SeatNo / col)+1 !=8) && (temp->data.SeatNo%col==0))?(temp->data.SeatNo/col):((temp->data.SeatNo/col)+1),
            ((temp->data.SeatNo % col)==0)?10:(temp->data.SeatNo % col),
            temp->data.Amtpaid,
            temp->data.date,
            temp->data.time,
            temp->data.snacks
        );
        strcat(buffer, line);
        snprintf(line2,sizeof(line2),"-------------------------------------------------\n");
        strcat(buffer,line2);
        temp = temp->next;
    }
    return buffer; // Python can read this string
}

char* searchtickets(char sname[])
{
    if(head == NULL) {
        char* empty = (char*)malloc(50);
        strcpy(empty, "No bookings yet!");
        return empty;
    }

    // estimate buffer size
    char* buffer = (char*)malloc(5000);
    buffer[0] = '\0';
    int found = 0;
    Node* temp = head;
    char line[200];
    char line1[200];
    char line2[200];
    snprintf(line1,sizeof(line1),"\n---------------- Search Results ----------------\n");
    strcat(buffer, line1);
    while(temp!=NULL)
    {
        if(stricmp(temp->data.name,sname)==0)
        {
            printf("\n Match found %d",found);
            found++;
            snprintf(line, sizeof(line),
            "Name:%s\nSeatNo:%d(Row:%d;Col:%d)\nAmount:%.2f\nDate:%s \nTime:%s\nSnacks:%c\n",
            temp->data.name,
            temp->data.SeatNo,
            ((temp->data.SeatNo / col) +1== 8) ? 7 : (((temp->data.SeatNo / col)+1 !=8) && (temp->data.SeatNo%col==0))?(temp->data.SeatNo/col):((temp->data.SeatNo/col)+1),
            ((temp->data.SeatNo % col)==0)?10:(temp->data.SeatNo % col),
            temp->data.Amtpaid,
            temp->data.date,
            temp->data.time,
            temp->data.snacks);
            strcat(buffer, line);
            snprintf(line2,sizeof(line2),"-------------------------------------------------\n");
            strcat(buffer,line2);
        }


        temp = temp->next;

    }
    if(found==0)
    {
        strcpy(buffer,"No booking found");
    }
    return buffer;
}
/*void bookhis(){
    if(head==NULL) {
        printf("\nNo bookings yet!\n");
        return;
    }

    Node*temp=head;
    printf("\n---------------- Booking History ----------------\n");
    while(temp != NULL) {
        printf("Name   : %s\n", temp->data.name);
        printf("SeatNo : %d (Row %d, Col %d)\n",
               temp->data.SeatNo,
               ((temp->data.SeatNo / col) +1== 8) ? 7 : (((temp->data.SeatNo / col)+1 !=8) && (temp->data.SeatNo%col==0))?(temp->data.SeatNo/col):((temp->data.SeatNo/col)+1),
               ((temp->data.SeatNo % col)==0)?10:(temp->data.SeatNo % col));
        printf("Amount : %.2f\n", temp->data.Amtpaid);
        printf("Date   : %s\n", temp->data.date);
        printf("Time   : %s\n", temp->data.time);
        printf("Snacks : %c \n",temp->data.snacks);
        printf("-------------------------------------------------\n");
        temp = temp->next;
    }
}*/

/*void SearchBookingByName() {
    if(head==NULL) {
        printf("\nNo bookings yet!\n");
        return;
    }

    char searchName[20];
    printf("Enter name to search: ");
    fflush(stdin);
    gets(searchName);
    Node*temp=head;
    int found = 0;

    printf("\n---------------- Search Results ----------------\n");
    while(temp != NULL){
        if(stricmp(temp->data.name, searchName)==0){
            printf("Name   : %s\n", temp->data.name);
            printf("SeatNo : %d (Row %d, Col %d)\n",
                   temp->data.SeatNo,
                   temp->data.SeatNo / col,
                   temp->data.SeatNo % col);
            printf("Amount : %.2f\n", temp->data.Amtpaid);
            printf("Date   : %s\n", temp->data.date);
            printf("Time   : %s\n", temp->data.time);
            printf("-------------------------------------------------\n");
            found = 1;
        }
        temp=temp->next;
    }
    if(!found){
        printf("No booking found for name: %s\n", searchName);
    }
}*/

int TotalSeatsBooked()
{
    int count=0;
    Node*temp=head;
    while(temp!=NULL) {
        count++;
        temp=temp->next;
    }
    printf("Total Seats Booked: %d\n", count);
    return count;
}

int TotalRevenue() {
    if(head == NULL){
        printf("\nNo bookings yet! Total revenue is Rs 0.00\n");
        return 0;
    }
    float total = 0;
    Node*temp = head;

    while (temp != NULL) {
        total += temp->data.Amtpaid;
        temp = temp->next;
    }
    printf("\n-----------------------------------------\n");
    printf("Total Seats Booked : ");
    int count =0;
    temp=head;
    while(temp !=NULL) {
        count++;
        temp =temp->next;
    }
    printf("%d\n",count);
    printf("Total Revenue Collected : Rs. %.2f\n", total);
    printf("-----------------------------------------\n");
    return total;
}

int* showtheater(){
printf("\nShowing theater...\n");
int seatWidth = 6;
int consoleWidth = 188;
int rowWidth=col*seatWidth;
int leftPadding=(consoleWidth-rowWidth)/2;
printf("\n\n\n\n");
printf("                                      -----------------------------------------------------------------------------------------");
printf("                                                                                                         *Screen*");
printf("\n\n\n\n\n\n\n\n");
for(int i=0;i<row;i++){
 for(int k=0; k<leftPadding; k++){
  printf(" ");
}
  for(int j=0;j<col;j++){
    printf("[%d]", seats[i][j]);
}
printf("\n");
}
printf("Legends\n");
printf("\n");
printf("1) 0=Vacant\n");
printf("2) 1=Reserved\n");
printf("\n\n");
return &seats[0][0];
}

int main()
{
    int R,C;
    char name[20];
    char snacks;
    char* buffer;


    printf("\n\n");
    printf("Cinema Seat Reservation");
    printf("\n\n");
    int choice=0;
    do{
        printf("\n\n       MENU\n");
        printf("\n\n");
        printf("1. View Seats\n");
        printf("2. Book a Seat\n");
        printf("3. Cancel Seat Reservation\n");
        printf("4. Booking History\n");
        printf("5. Search Booking By Name\n");
        printf("6. Total Seats Booked\n");
        printf("7. Total Revenue\n");
        printf("8. exit\n");
        printf("\n\n");
        printf("Enter Your Choice:-");
        scanf("%d", &choice);
    switch(choice){
     case 1:
      showtheater();
      break;

     case 2:
        fflush(stdin);
        printf("\n Enter Your Name:-");
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        fflush(stdin);
        fgets(name,20,stdin);
        printf("\n Enter The Row & Column For The Seat Which You Want To Book:-");
        scanf("%d %d",&R,&C);
        printf("\n do you want snacks Y/N?");
        fflush(stdin);
        while ((ch = getchar()) != '\n' && ch != EOF);
        scanf("%c",&snacks);
        snacks=toupper(snacks);
        if(snacks!='Y'&& snacks!='N')
        {
            snacks='N';
        }
        bookseat(R,C,name,snacks);
        break;

     case 3:
            printf("Enter the Row and Column of the seat you want to cancel:-");
            scanf("%d %d", &R, &C);
            cancelseat(R,C);
            break;

     case 4:
        printf("\n Calling new function");
        buffer=bookhistory();
        printf("\n---------------- Booking History ----------------\n%s",buffer);

        //bookhis();
        break;

     case 5:
         printf("\n please enter the name by which booking is to be searched:");
         fflush(stdin);
         fgets(name,100,stdin);
         buffer=searchtickets(name);
         printf("\n---------------- Search Results ----------------\n%s",buffer);
        //SearchBookingByName();
        break;

     case 6:
        TotalSeatsBooked();
        break;

     case 7:
        TotalRevenue();
        break;

     case 8:
        printf("\nExiting the program. Thank you!\n");
        break;
    default:
        printf("Invalid choice! Please try again.\n");
}
}
while(choice!=8);
return 0;
}
