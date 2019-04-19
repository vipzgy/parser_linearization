# parser_linearization

## environment
ubuntu 16.04  
java 1.8

## operation
cd maltparser-1.9.1  
update parser_linearization.sh according to your needs  
chmod +x ./parser_linearization.sh
./parser_linearization.sh

cd extractDepAction  
mkdir build  
cmake ..  
make  
update extra_action.sh according to your needs  
chmod +x ./extra_action.sh  
./extra_action.sh
