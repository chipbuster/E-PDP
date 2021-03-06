CC = cc
CFLAGS = 
LIBS	= libpc.a -lm -lcurses -ltermlib 
SOURCES = patterns.c main.c variable.c template.c general.c display.c io.c command.c 
OBJECTS = patterns.o main.o variable.o template.o general.o display.o io.o command.o 
AADEST = ../aa
BPDEST = ../bp
CLDEST = ../cl
IADEST = ../ia
IACDEST = ../iac
CSDEST = ../cs
PADEST = ../pa
PLOTDEST = ../utils
COLEXDEST = ../utils

.SUFFIXES : .h

.h.h :
	touch $<

progs:	$(AADEST)/aa $(BPDEST)/bp $(CLDEST)/cl $(CSDEST)/cs $(IADEST)/ia $(IACDEST)/iac $(PADEST)/pa 
	
aa:	$(AADEST)/aa

bp:	$(BPDEST)/bp

cl:	$(CLDEST)/cl

cs:	$(CSDEST)/cs

ia:	$(IADEST)/ia

iac:	$(IACDEST)/iac

pa:	$(PADEST)/pa

plot:	$(PLOTDEST)/plot

colex:	$(COLEXDEST)/colex

$(BPDEST)/bp: bp.o weights.o libpc.a
	$(CC) $(CFLAGS) bp.o weights.o $(LIBS) -o $(BPDEST)/bp

$(CSDEST)/cs: cs.o weights.o libpc.a
	$(CC) $(CFLAGS) cs.o weights.o $(LIBS) -o $(CSDEST)/cs

$(CLDEST)/cl: cl.o libpc.a
	$(CC) $(CFLAGS) cl.o  $(LIBS) -o $(CLDEST)/cl

$(AADEST)/aa:  aa.o libpc.a
	$(CC) $(CFLAGS) aa.o  $(LIBS) -o $(AADEST)/aa

$(IADEST)/ia: ia.o iaaux.o iatop.o libpc.a
	$(CC) $(CFLAGS) ia.o iaaux.o iatop.o $(LIBS) -o $(IADEST)/ia

$(PADEST)/pa: pa.o weights.o libpc.a
	$(CC) $(CFLAGS) pa.o weights.o $(LIBS) -o $(PADEST)/pa

$(IACDEST)/iac: iac.o weights.o libpc.a
	$(CC) $(CFLAGS) iac.o weights.o $(LIBS) -o $(IACDEST)/iac

libpc.a: $(OBJECTS)
	ar rv libpc.a patterns.o main.o variable.o \
		template.o general.o display.o io.o command.o 
	ranlib libpc.a


utils:  plot colex

$(PLOTDEST)/plot:	plot.c
	cc plot.c -o $(PLOTDEST)/plot -lm
	
$(COLEXDEST)/colex:	colex.c
	cc colex.c -o $(COLEXDEST)/colex
	
strip:
	strip $(BPDEST)/bp $(CSDEST)/cs $(CLDEST)/cl \
	$(AADEST)/aa $(IADEST)/ia $(PADEST)/pa $(IACDEST)/iac \
	$(COLEXDEST)/colex $(PLOTDEST)/plot
	
rm:
	rm $(BPDEST)/bp $(CSDEST)/cs $(CLDEST)/cl \
	$(AADEST)/aa $(IADEST)/ia $(PADEST)/pa $(IACDEST)/iac \
	$(COLEXDEST)/colex $(PLOTDEST)/plot
	
clean:
	rm *.o libpc.a
	
lint_all: lint_bp lint_cs lint_cl \
	lint_mdm lint_ia lint_pa lint_iac

lint_bp:
	lint $(SOURCES) bp.c weights.c

lint_cs:
	lint $(SOURCES) cs.c

lint_cl:
	lint $(SOURCES) cl.c

lint_aa:
	lint $(SOURCES) aa.c

lint_ia:
	lint $(SOURCES) ia.c iatop.c iaaux.c

lint_pa:
	lint $(SOURCES) pa.c weights.c

lint_iac:
	lint $(SOURCES) iac.c weights.c
	
depend:
	grep '^#[ 	]*include[ 	]*"' *.c | \
	sed -e 's/:[^"]*"\([^"]*\)"/:	\1/' -e 's/\.c/.o/' | \
	awk ' { if ($$1 != prev) { print rec; rec = $$0; prev = $$1; } \
		else { if (length(rec $$2) > 78) { print rec; rec = $$0; } \
		       else rec = rec " " $$2 } } \
	      END { print rec } ' > makedep
	grep '^#[ 	]*include[ 	]*"' *.h | \
	sed -e 's/:[^"]*"\([^"]*\)"/:	\1/' | \
	awk ' BEGIN { prev = " "} \
	   { if (prev == " ") { rec = $$0; prev = $$1;} \
	      else { if ($$1 != prev) {print rec; rec = $$0; prev = $$1;} \
		else { if (length(rec $$2) > 78) { print rec; rec = $$0; } \
		       else rec = rec " " $$2 } } } \
	    END {print rec;} '  > hmakedep
	echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
	echo '$$r hmakedep' >>eddep
	echo '$$r makedep' >>eddep
	echo 'w' >>eddep
	cp Makefile Makefile.bak
	ed - Makefile < eddep
	rm eddep makedep hmakedep
	echo '' >> Makefile
	echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
	echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
	echo '# see make depend above' >> Makefile

# DO NOT DELETE THIS LINE -- make depend uses it

general.h:	display.h

aa.o:	general.h aa.h variable.h patterns.h command.h
bp.o:	general.h bp.h variable.h weights.h patterns.h command.h
cl.o:	general.h variable.h patterns.h command.h cl.h
command.o:	general.h io.h command.h
cs.o:	general.h cs.h variable.h command.h patterns.h weights.h
display.o:	general.h io.h variable.h template.h weights.h command.h
# func.o:	general.h command.h patterns.h variable.h weights.h
general.o:	general.h command.h variable.h
ia.o:	ia.h io.h general.h
iaaux.o:	ia.h
iac.o:	general.h iac.h variable.h command.h weights.h patterns.h
iatop.o:	general.h cs.h variable.h command.h ia.h
io.o:	io.h
jbp.o:	general.h bp.h variable.h weights.h patterns.h command.h
main.o:	general.h variable.h command.h patterns.h
pa.o:	general.h pa.h variable.h weights.h patterns.h command.h
patterns.o:	general.h command.h variable.h patterns.h
template.o:	general.h command.h variable.h display.h template.h
variable.o:	general.h variable.h command.h patterns.h weights.h
weights.o:	general.h command.h weights.h variable.h

# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
