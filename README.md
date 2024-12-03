# C_sql_injection
 This is an case study for  demonstrating sql injection in C

# Mysql installation Kali Linux:
https://computingforgeeks.com/how-to-install-mysql-on-kali-linux/

# Plugins for C API:
1. sudo apt install libmysqlclient-dev
2. mysql_config --cflags
3. mysql_config --libs

# Linux Mysql implementation reference:
https://dev.mysql.com/doc/c-api/9.1/en/c-api-building-clients.html

# build(three version):
1. make DESIGNER: This version can be used as a normal mysql interface
2. make LOGIN: This version has vulnerable architecture for sql injection
3. make PARAMETERIZED: This version uses parameterized queries to protection sql injection


# clean:
make clean


