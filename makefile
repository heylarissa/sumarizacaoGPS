CFLAGS = -Wall -g  # gerar "warnings" detalhados e infos de depuração
 
objs = gps.o readlogs.o menu.o

# regra default (primeira regra)
all: gps
 
# regras de ligacao
gps: $(objs)
 
# regras de compilação
gps.o:   gps.c logs.h
readlogs.o: readlogs.c logs.h
menu.o: menu.c logs.h
 
# remove arquivos temporários
clean:
	-rm -f $(objs) *~
 
# remove tudo o que não for o código-fonte
purge: clean
	-rm -f gps