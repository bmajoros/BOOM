CC		= g++
C++		= g++
DEBUG		= -g
OPTIMIZE	= -O
DEFINES		= $(OPTIMIZE) -fpermissive -w
LDFLAGS		= $(OPTIMIZE)
BOOM		= BOOM
OBJ		= obj
LIBS		= -LBOOM -lBOOM

$(OBJ):
	mkdir $(OBJ)

clean:
	rm obj/*.o

#------------------------------------------------------------------------------
obj/globals.o: \
		globals.C \
		globals.H \
		ObjectPrinter.H \
		Evaluator.H \
		RunTimeEnvironment.H \
		Parser.H \
		LambdaException.H \
		AstPrinter.H \
		AST.H \
		LambdaObject.H \
		Garbage.H \
		FunctionObject.H \
		ForeignFunctionObject.H \
		Closure.H \
		lambda.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/globals.o \
		globals.C
#------------------------------------------------------------------------------
obj/testapi.o: \
		testapi.C \
		globals.H \
		LambdaAPI.H \
		ObjectPrinter.H \
		Evaluator.H \
		RunTimeEnvironment.H \
		Parser.H \
		LambdaException.H \
		AstPrinter.H \
		AST.H \
		LambdaObject.H \
		Garbage.H \
		FunctionObject.H \
		ForeignFunctionObject.H \
		Closure.H \
		lambda.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/testapi.o \
		testapi.C
#------------------------------------------------------------------------------
obj/LambdaAPI.o: \
		LambdaAPI.H \
		LambdaAPI.C \
		LambdaObject.H \
		Garbage.H \
		MarkHook.H \
		linked.H \
		RunTimeEnvironment.H \
		RunTimeStack.H \
		ScopeStack.H \
		AR.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/LambdaAPI.o \
		LambdaAPI.C
#------------------------------------------------------------------------------
obj/MarkStack.o: \
		MarkStack.C \
		MarkStack.H \
		Garbage.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/MarkStack.o \
		MarkStack.C
#------------------------------------------------------------------------------
obj/LambdaVector.o: \
		LambdaVector.H \
		LambdaVector.C
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/LambdaVector.o \
		LambdaVector.C
#------------------------------------------------------------------------------
obj/Lists.o: \
		Lists.H \
		Lists.C \
		ConsCell.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/Lists.o \
		Lists.C
#------------------------------------------------------------------------------
obj/ObjectPrinter.o: \
		ObjectPrinter.H \
		LambdaObject.H \
		LambdaInt.H \
		LambdaFloat.H \
		LambdaString.H \
		LambdaChar.H \
		ObjectPrinter.C
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/ObjectPrinter.o \
		ObjectPrinter.C
#------------------------------------------------------------------------------
obj/Evaluator.o: \
		Evaluator.H \
		LambdaInt.H \
		LambdaFloat.H \
		LambdaString.H \
		LambdaChar.H \
		AST.H \
		Evaluator.C
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/Evaluator.o \
		Evaluator.C
#------------------------------------------------------------------------------
obj/FunctionObject.o: \
		FunctionObject.C \
		FunctionObject.H \
		LambdaObject.H \
		Garbage.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/FunctionObject.o \
		FunctionObject.C
#------------------------------------------------------------------------------
obj/ForeignFunctionObject.o: \
		ForeignFunctionObject.C \
		ForeignFunctionObject.H \
		FunctionObject.H \
		LambdaObject.H \
		Garbage.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/ForeignFunctionObject.o \
		ForeignFunctionObject.C
#------------------------------------------------------------------------------
obj/LambdaChar.o: \
		LambdaChar.C \
		LambdaChar.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/LambdaChar.o \
		LambdaChar.C
#------------------------------------------------------------------------------
obj/LambdaString.o: \
		LambdaString.H \
		LambdaString.C
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/LambdaString.o \
		LambdaString.C
#------------------------------------------------------------------------------
obj/LambdaInt.o: \
		LambdaInt.C \
		LambdaInt.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/LambdaInt.o \
		LambdaInt.C
#------------------------------------------------------------------------------
obj/LambdaFloat.o: \
		LambdaFloat.C \
		LambdaFloat.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/LambdaFloat.o \
		LambdaFloat.C
#------------------------------------------------------------------------------
obj/Closure.o: \
		Closure.C \
		Closure.H \
		FunctionObject.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/Closure.o \
		Closure.C
#------------------------------------------------------------------------------
obj/RunTimeStack.o: \
		RunTimeStack.C \
		RunTimeStack.H \
		AR.H \
		LambdaObject.H \
		linked.H \
		Garbage.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/RunTimeStack.o \
		RunTimeStack.C
#------------------------------------------------------------------------------
obj/GC.o: \
		GC.C \
		GC.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/GC.o \
		GC.C
#------------------------------------------------------------------------------
obj/AR.o: \
		AR.C \
		AR.H \
		Garbage.H \
		LambdaObject.H \
		linked.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/AR.o \
		AR.C
#------------------------------------------------------------------------------
obj/Garbage.o: \
		Garbage.C \
		Garbage.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/Garbage.o \
		Garbage.C
#------------------------------------------------------------------------------
obj/LambdaObject.o: \
		LambdaObject.C \
		LambdaObject.H \
		Garbage.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/LambdaObject.o \
		LambdaObject.C
#------------------------------------------------------------------------------
obj/ConsCell.o: \
		ConsCell.C \
		ConsCell.H \
		LambdaObject.H \
		Garbage.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/ConsCell.o \
		ConsCell.C
#------------------------------------------------------------------------------
obj/AstSerializer.o: \
		AstSerializer.C \
		AstSerializer.H \
		AstVisitor.H \
		AST.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/AstSerializer.o \
		AstSerializer.C
#------------------------------------------------------------------------------
obj/AstVisitor.o: \
		AstVisitor.C \
		AstVisitor.H \
		AstVisitor.H \
		AST.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/AstVisitor.o \
		AstVisitor.C
#------------------------------------------------------------------------------
obj/AstPrinter.o: \
		AstPrinter.C \
		AstPrinter.H \
		AstVisitor.H \
		AST.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/AstPrinter.o \
		AstPrinter.C
#------------------------------------------------------------------------------
obj/ScopeStack.o: \
		ScopeStack.C \
		ScopeStack.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/ScopeStack.o \
		ScopeStack.C
#------------------------------------------------------------------------------
obj/RunTimeEnvironment.o: \
		RunTimeEnvironment.C \
		RunTimeEnvironment.H \
		RunTimeStack.H \
		ScopeStack.H \
		SymbolRepository.H \
		AR.H \
		LambdaObject.H \
		Garbage.H \
		linked.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/RunTimeEnvironment.o \
		RunTimeEnvironment.C
#------------------------------------------------------------------------------
obj/SymbolRepository.o: \
		SymbolRepository.C \
		SymbolRepository.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/SymbolRepository.o \
		SymbolRepository.C
#------------------------------------------------------------------------------
obj/Symbol.o: \
		Symbol.C \
		LambdaObject.H \
		Symbol.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/Symbol.o \
		Symbol.C
#------------------------------------------------------------------------------
obj/LexicalAddress.o: \
		LexicalAddress.C \
		LexicalAddress.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/LexicalAddress.o \
		LexicalAddress.C
#------------------------------------------------------------------------------
obj/TestParser.o: \
		TestParser.C \
		Scanner.H \
		Token.H \
		TokenStream.H \
		RunTimeEnvironment.H \
		Parser.H \
		LambdaException.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/TestParser.o \
		TestParser.C
#------------------------------------------------------------------------------
obj/AST.o: \
		AST.H \
		AST.C
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/AST.o \
		AST.C
#------------------------------------------------------------------------------
obj/Parser.o: \
		Parser.H \
		Parser.C \
		TokenStream.H \
		Scanner.H \
		Token.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/Parser.o \
		Parser.C
#------------------------------------------------------------------------------
obj/TokenStream.o: \
		TokenStream.C \
		TokenStream.H \
		Token.H \
		Scanner.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/TokenStream.o \
		TokenStream.C
#------------------------------------------------------------------------------
obj/linked.o: \
		linked.C \
		linked.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/linked.o \
		linked.C
#------------------------------------------------------------------------------
obj/LambdaException.o: \
		LambdaException.C \
		LambdaException.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/LambdaException.o \
		LambdaException.C
#------------------------------------------------------------------------------
obj/Scanner.o: \
		Scanner.C \
		Scanner.H \
		Token.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/Scanner.o \
		Scanner.C
#------------------------------------------------------------------------------
obj/Token.o: \
		Token.C \
		Token.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/Token.o \
		Token.C
#------------------------------------------------------------------------------
obj/CommandLine.o: \
		CommandLine.C \
		CommandLine.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/CommandLine.o \
		CommandLine.C
#------------------------------------------------------------------------------
obj/lambda.o: \
		lambda.C \
		ObjectPrinter.H \
		Evaluator.H \
		RunTimeEnvironment.H \
		Parser.H \
		LambdaException.H \
		AstPrinter.H \
		AST.H \
		LambdaObject.H \
		Garbage.H \
		FunctionObject.H \
		ForeignFunctionObject.H \
		Closure.H \
		lambda.H
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/lambda.o \
		lambda.C
#------------------------------------------------------------------------------
obj/TestScanner.o: \
		TestScanner.C
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/TestScanner.o \
		TestScanner.C
#------------------------------------------------------------------------------
obj/SymbolTable.o: \
		SymbolTable.C
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/SymbolTable.o \
		SymbolTable.C
#------------------------------------------------------------------------------
obj/MarkHook.o: \
		MarkHook.C
	$(C++) -c $(DEFINES) $(INCLUDES) -o obj/MarkHook.o \
		MarkHook.C
#------------------------------------------------------------------------------
TestScanner: \
		obj/TestScanner.o \
		obj/LambdaException.o \
		obj/Token.o \
		obj/Scanner.o \
		obj/Random.o \
		obj/HashTable.o 
	$(C++) $(LDFLAGS)  -o TestScanner \
		obj/TestScanner.o \
		obj/LambdaException.o \
		obj/Token.o \
		obj/Scanner.o \
		obj/Random.o \
		obj/HashTable.o \
		$(LIBS)
#------------------------------------------------------------------------------
TestParser: \
		obj/TestParser.o \
		obj/Garbage.o \
		obj/LambdaObject.o \
		obj/ConsCell.o \
		obj/AstPrinter.o \
		obj/linked.o \
		obj/SymbolRepository.o \
		obj/RunTimeEnvironment.o \
		obj/Symbol.o \
		obj/LexicalAddress.o \
		obj/AST.o \
		obj/Parser.o \
		obj/TokenStream.o \
		obj/LambdaException.o \
		obj/Token.o \
		obj/Scanner.o \
		obj/Random.o \
		obj/HashTable.o
	$(C++) $(LDFLAGS)  -o TestParser \
		obj/TestParser.o \
		obj/Garbage.o \
		obj/LambdaObject.o \
		obj/ConsCell.o \
		obj/AstPrinter.o \
		obj/linked.o \
		obj/RunTimeEnvironment.o \
		obj/SymbolRepository.o \
		obj/Symbol.o \
		obj/LexicalAddress.o \
		obj/AST.o \
		obj/Parser.o \
		obj/TokenStream.o \
		obj/LambdaException.o \
		obj/Token.o \
		obj/Scanner.o \
		obj/Random.o \
		obj/HashTable.o
		$(LIBS)
#------------------------------------------------------------------------------
lambda: \
		obj/linked.o \
		obj/MarkHook.o \
		obj/lambda.o \
		obj/ScopeStack.o \
		obj/SymbolTable.o \
		obj/CommandLine.o \
		obj/globals.o \
		obj/LambdaVector.o \
		obj/MarkStack.o \
		obj/Lists.o \
		obj/ObjectPrinter.o \
		obj/Evaluator.o \
		obj/LambdaChar.o \
		obj/FunctionObject.o \
		obj/ForeignFunctionObject.o \
		obj/LambdaInt.o \
		obj/LambdaFloat.o \
		obj/LambdaString.o \
		obj/Closure.o \
		obj/RunTimeStack.o \
		obj/GC.o \
		obj/AR.o \
		obj/Garbage.o \
		obj/LambdaObject.o \
		obj/ConsCell.o \
		obj/AstPrinter.o \
		obj/SymbolRepository.o \
		obj/RunTimeEnvironment.o \
		obj/Symbol.o \
		obj/LexicalAddress.o \
		obj/AST.o \
		obj/Parser.o \
		obj/TokenStream.o \
		obj/LambdaException.o \
		obj/Token.o \
		obj/Scanner.o
	$(C++) $(LDFLAGS)  -o lambda \
		obj/linked.o \
		obj/MarkHook.o \
		obj/lambda.o \
		obj/ScopeStack.o \
		obj/SymbolTable.o \
		obj/CommandLine.o \
		obj/globals.o \
		obj/MarkStack.o \
		obj/LambdaVector.o \
		obj/Lists.o \
		obj/ObjectPrinter.o \
		obj/Evaluator.o \
		obj/LambdaChar.o \
		obj/FunctionObject.o \
		obj/ForeignFunctionObject.o \
		obj/LambdaInt.o \
		obj/LambdaFloat.o \
		obj/LambdaString.o \
		obj/Closure.o \
		obj/RunTimeStack.o \
		obj/GC.o \
		obj/AR.o \
		obj/Garbage.o \
		obj/LambdaObject.o \
		obj/ConsCell.o \
		obj/AstPrinter.o \
		obj/RunTimeEnvironment.o \
		obj/SymbolRepository.o \
		obj/Symbol.o \
		obj/LexicalAddress.o \
		obj/AST.o \
		obj/Parser.o \
		obj/TokenStream.o \
		obj/LambdaException.o \
		obj/Token.o \
		obj/Scanner.o \
		$(LIBS)
#------------------------------------------------------------------------------
testapi: \
		obj/testapi.o \
		obj/MarkHook.o \
		obj/AstSerializer.o \
		obj/LambdaAPI.o \
		obj/globals.o \
		obj/LambdaVector.o \
		obj/MarkStack.o \
		obj/Lists.o \
		obj/ObjectPrinter.o \
		obj/Evaluator.o \
		obj/LambdaChar.o \
		obj/FunctionObject.o \
		obj/ForeignFunctionObject.o \
		obj/LambdaInt.o \
		obj/LambdaFloat.o \
		obj/LambdaString.o \
		obj/Closure.o \
		obj/RunTimeStack.o \
		obj/GC.o \
		obj/AR.o \
		obj/Garbage.o \
		obj/LambdaObject.o \
		obj/ConsCell.o \
		obj/AstPrinter.o \
		obj/linked.o \
		obj/SymbolRepository.o \
		obj/RunTimeEnvironment.o \
		obj/Symbol.o \
		obj/LexicalAddress.o \
		obj/AST.o \
		obj/Parser.o \
		obj/TokenStream.o \
		obj/LambdaException.o \
		obj/Token.o \
		obj/Scanner.o \
		obj/Random.o \
		obj/HashTable.o
	$(C++) $(LDFLAGS)  -o testapi \
		obj/testapi.o \
		obj/MarkHook.o \
		obj/AstSerializer.o \
		obj/LambdaAPI.o \
		obj/globals.o \
		obj/MarkStack.o \
		obj/LambdaVector.o \
		obj/Lists.o \
		obj/ObjectPrinter.o \
		obj/Evaluator.o \
		obj/LambdaChar.o \
		obj/FunctionObject.o \
		obj/ForeignFunctionObject.o \
		obj/LambdaInt.o \
		obj/LambdaFloat.o \
		obj/LambdaString.o \
		obj/Closure.o \
		obj/RunTimeStack.o \
		obj/GC.o \
		obj/AR.o \
		obj/Garbage.o \
		obj/LambdaObject.o \
		obj/ConsCell.o \
		obj/AstPrinter.o \
		obj/linked.o \
		obj/RunTimeEnvironment.o \
		obj/SymbolRepository.o \
		obj/Symbol.o \
		obj/LexicalAddress.o \
		obj/AST.o \
		obj/Parser.o \
		obj/TokenStream.o \
		obj/LambdaException.o \
		obj/Token.o \
		obj/Scanner.o \
		obj/Random.o \
		obj/HashTable.o \
		$(LIBS)
#------------------------------------------------------------------------------
liblambda.a: \
		obj \
		obj/AstVisitor.o \
		obj/LambdaAPI.o \
		obj/MarkHook.o \
		obj/linked.o \
		obj/lambda.o \
		obj/ScopeStack.o \
		obj/SymbolTable.o \
		obj/CommandLine.o \
		obj/globals.o \
		obj/LambdaVector.o \
		obj/MarkStack.o \
		obj/Lists.o \
		obj/ObjectPrinter.o \
		obj/Evaluator.o \
		obj/LambdaChar.o \
		obj/FunctionObject.o \
		obj/ForeignFunctionObject.o \
		obj/LambdaInt.o \
		obj/LambdaFloat.o \
		obj/LambdaString.o \
		obj/Closure.o \
		obj/RunTimeStack.o \
		obj/GC.o \
		obj/AR.o \
		obj/Garbage.o \
		obj/LambdaObject.o \
		obj/ConsCell.o \
		obj/AstPrinter.o \
		obj/SymbolRepository.o \
		obj/RunTimeEnvironment.o \
		obj/Symbol.o \
		obj/LexicalAddress.o \
		obj/AST.o \
		obj/Parser.o \
		obj/TokenStream.o \
		obj/LambdaException.o \
		obj/Token.o \
		obj/Scanner.o
	ar -s -r liblambda.a \
		obj/AstVisitor.o \
		obj/LambdaAPI.o \
		obj/MarkHook.o \
		obj/linked.o \
		obj/lambda.o \
		obj/ScopeStack.o \
		obj/SymbolTable.o \
		obj/CommandLine.o \
		obj/globals.o \
		obj/MarkStack.o \
		obj/LambdaVector.o \
		obj/Lists.o \
		obj/ObjectPrinter.o \
		obj/Evaluator.o \
		obj/LambdaChar.o \
		obj/FunctionObject.o \
		obj/ForeignFunctionObject.o \
		obj/LambdaInt.o \
		obj/LambdaFloat.o \
		obj/LambdaString.o \
		obj/Closure.o \
		obj/RunTimeStack.o \
		obj/GC.o \
		obj/AR.o \
		obj/Garbage.o \
		obj/LambdaObject.o \
		obj/ConsCell.o \
		obj/AstPrinter.o \
		obj/RunTimeEnvironment.o \
		obj/SymbolRepository.o \
		obj/Symbol.o \
		obj/LexicalAddress.o \
		obj/AST.o \
		obj/Parser.o \
		obj/TokenStream.o \
		obj/LambdaException.o \
		obj/Token.o \
		obj/Scanner.o
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------



