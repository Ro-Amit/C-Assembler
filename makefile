assembler: object_files/assembler.o object_files/preprocess.o object_files/firstpass.o object_files/secondpass.o object_files/handle_instructions.o object_files/helpers.o \
     object_files/memory.o object_files/symbol_table.o object_files/macro_list.o object_files/struct_manager.o object_files/instructions_details.o \
     object_files/output_maker.o
		gcc -ansi -pedantic -Wall -g -o assembler object_files/assembler.o object_files/preprocess.o object_files/firstpass.o object_files/secondpass.o \
              object_files/handle_instructions.o object_files/helpers.o object_files/memory.o object_files/symbol_table.o object_files/macro_list.o object_files/struct_manager.o \
              object_files/instructions_details.o object_files/output_maker.o
object_files/assembler.o: assembler.c assembler.h \
  data_structures/headers/instructions_details.h \
  data_structures/headers/../../header_files/globals.h \
  header_files/preprocess.h \
  header_files/../data_structures/headers/macro_list.h \
  header_files/../data_structures/headers/../../header_files/globals.h \
  header_files/../data_structures/headers/struct_manager.h \
  header_files/../data_structures/headers/symbol_table.h \
  header_files/../data_structures/headers/memory.h \
  header_files/../data_structures/headers/instructions_details.h \
  header_files/globals.h header_files/helpers.h header_files/firstpass.h \
  header_files/handle_instructions.h data_structures/headers/memory.h \
  data_structures/headers/symbol_table.h \
  data_structures/headers/macro_list.h header_files/secondpass.h \
  data_structures/headers/struct_manager.h header_files/output_maker.h
	gcc -ansi -pedantic -Wall -g -c assembler.c -o object_files/assembler.o

object_files/preprocess.o: source_files/preprocess.c \
  source_files/../header_files/preprocess.h \
  source_files/../header_files/../data_structures/headers/macro_list.h \
  source_files/../header_files/../data_structures/headers/../../header_files/globals.h \
  source_files/../header_files/../data_structures/headers/struct_manager.h \
  source_files/../header_files/../data_structures/headers/symbol_table.h \
  source_files/../header_files/../data_structures/headers/memory.h \
  source_files/../header_files/../data_structures/headers/instructions_details.h \
  source_files/../header_files/globals.h \
  source_files/../header_files/helpers.h
	gcc -ansi -pedantic -Wall -g -c ./source_files/preprocess.c -o object_files/preprocess.o

object_files/firstpass.o: source_files/firstpass.c \
  source_files/../header_files/firstpass.h \
  source_files/../header_files/globals.h \
  source_files/../header_files/helpers.h \
  source_files/../header_files/../data_structures/headers/memory.h \
  source_files/../header_files/../data_structures/headers/symbol_table.h \
  source_files/../header_files/../data_structures/headers/../../header_files/globals.h \
  source_files/../header_files/../data_structures/headers/macro_list.h \
  source_files/../header_files/../data_structures/headers/struct_manager.h \
  source_files/../header_files/../data_structures/headers/instructions_details.h \
  source_files/../header_files/handle_instructions.h
	gcc -ansi -pedantic -Wall -g -c ./source_files/firstpass.c -o object_files/firstpass.o

object_files/secondpass.o: source_files/secondpass.c \
  source_files/../header_files/secondpass.h \
  source_files/../header_files/helpers.h \
  source_files/../header_files/globals.h \
  source_files/../header_files/../data_structures/headers/memory.h \
  source_files/../header_files/../data_structures/headers/symbol_table.h \
  source_files/../header_files/../data_structures/headers/../../header_files/globals.h \
  source_files/../header_files/../data_structures/headers/macro_list.h \
  source_files/../header_files/handle_instructions.h \
  source_files/../header_files/../data_structures/headers/instructions_details.h \
  source_files/../header_files/../data_structures/headers/struct_manager.h
	gcc -ansi -pedantic -Wall -g -c ./source_files/secondpass.c -o object_files/secondpass.o

object_files/handle_instructions.o: source_files/handle_instructions.c \
  source_files/../header_files/handle_instructions.h \
  source_files/../header_files/helpers.h \
  source_files/../header_files/globals.h \
  source_files/../header_files/../data_structures/headers/memory.h \
  source_files/../header_files/../data_structures/headers/symbol_table.h \
  source_files/../header_files/../data_structures/headers/../../header_files/globals.h \
  source_files/../header_files/../data_structures/headers/macro_list.h \
  source_files/../header_files/../data_structures/headers/instructions_details.h
	gcc -ansi -pedantic -Wall -g -c ./source_files/handle_instructions.c -o object_files/handle_instructions.o

object_files/helpers.o: source_files/helpers.c source_files/../header_files/helpers.h \
  source_files/../header_files/../data_structures/headers/memory.h \
  source_files/../header_files/../data_structures/headers/symbol_table.h \
  source_files/../header_files/../data_structures/headers/../../header_files/globals.h \
  source_files/../header_files/../data_structures/headers/macro_list.h \
  source_files/../header_files/globals.h
	gcc -ansi -pedantic -Wall -g -c ./source_files/helpers.c -o object_files/helpers.o

object_files/memory.o: data_structures/source/memory.c \
  data_structures/source/../headers/memory.h
	gcc -ansi -pedantic -Wall -g -c ./data_structures/source/memory.c -o object_files/memory.o
object_files/symbol_table.o: data_structures/source/symbol_table.c \
  data_structures/source/../headers/symbol_table.h \
  data_structures/source/../headers/../../header_files/globals.h
	gcc -ansi -pedantic -Wall -g -c ./data_structures/source/symbol_table.c -o object_files/symbol_table.o

object_files/macro_list.o: data_structures/source/macro_list.c \
  data_structures/source/../headers/macro_list.h \
  data_structures/source/../headers/../../header_files/globals.h
	gcc -ansi -pedantic -Wall -g -c ./data_structures/source/macro_list.c -o object_files/macro_list.o

object_files/struct_manager.o: data_structures/source/struct_manager.c \
  data_structures/source/../headers/struct_manager.h \
  data_structures/source/../headers/macro_list.h \
  data_structures/source/../headers/../../header_files/globals.h \
  data_structures/source/../headers/symbol_table.h \
  data_structures/source/../headers/memory.h \
  data_structures/source/../headers/instructions_details.h
	gcc -ansi -pedantic -Wall -g -c ./data_structures/source/struct_manager.c -o object_files/struct_manager.o

object_files/instructions_details.o: data_structures/source/instructions_details.c \
  data_structures/source/../headers/instructions_details.h \
  data_structures/source/../headers/../../header_files/globals.h
	gcc -ansi -pedantic -Wall -g -c ./data_structures/source/instructions_details.c -o object_files/instructions_details.o

object_files/output_maker.o: source_files/output_maker.c \
  source_files/../header_files/output_maker.h \
  source_files/../header_files/globals.h \
  source_files/../header_files/../data_structures/headers/struct_manager.h \
  source_files/../header_files/../data_structures/headers/macro_list.h \
  source_files/../header_files/../data_structures/headers/../../header_files/globals.h \
  source_files/../header_files/../data_structures/headers/symbol_table.h \
  source_files/../header_files/../data_structures/headers/memory.h \
  source_files/../header_files/../data_structures/headers/instructions_details.h
	gcc -ansi -pedantic -Wall -g -c source_files/output_maker.c -o object_files/output_maker.o

clean:
	rm -f object_files/*.o assembler
