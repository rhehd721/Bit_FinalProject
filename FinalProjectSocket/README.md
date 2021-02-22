# Final Project

## Server
1. Client에게 Name+Command를 받아온다.
2. DB를 통해 Client_Name을 확인하여 해당 Raspberry를 찾아낸다.
3. Raspberry에게 Command를 보낸다.
4. Raspberry가 명령을 수행하고 수집된 정보를 받아온다.
5. 받아온 정보를 분석하고 DB에 저장한다.
6. 분석된 결과를 Client에게 보낸다.

## Client
1. 원하는 명령을 GUI Btn을 통해 Server에게 보낸다.

## Camera
1. Server에게 받은 명령을 바탕으로 임무를 수행한다.

## DB
1. User_Table (ID, PASS, RaspberryNo)
2. UserID_Inventory_Change_Table (Product_Name, Amount, ImagePath, Time)
3. UserID_Inventory_Result_Table (Product_Name, Amount, ImagePath, Time)