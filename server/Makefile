.PHONY : clean
INCLUDE=/usr/include
LIB=/usr/lib/mysql
OBJ=main.o ex_db.o base64.o
TAR=exam

%.o:%.c
	$(CC) -g -c $< -o $@  -I$(INCLUDE) -L$(LIB) -L /usr/lib/mysql -lmysqlclient

$(TAR):$(OBJ)
	$(CC)  -o $(TAR) $(OBJ)  -I$(INCLUDE) -L$(LIB) -L /usr/lib/mysql -lmysqlclient
clean:
	rm -f $(TAR) *.o
