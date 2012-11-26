#include <clang-c/Index.h>


CXChildVisitResult printAST(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
	CXString str;
	CXChildVisitResult ret = CXChildVisit_Recurse;

	if(clang_getCursorKind (cursor) != CXCursor_FunctionDecl) {
		ret = CXChildVisit_Continue;
		goto proc_exit;
	}
	
	str = clang_getCursorSpelling(cursor);
	printf("Function Name:\t%s\n", clang_getCString(str));
	str = clang_getCursorDisplayName(cursor);
	printf("Display Name :\t%s\n", clang_getCString(str));
	printf("No. of Args  :\t%d\n", clang_Cursor_getNumArguments(cursor));
	printf("---------------------------------------\n");
proc_exit:
	return ret;
}

int main(int argc, char **argv)
{
	CXIndex Index = clang_createIndex(0, 0);
	CXTranslationUnit TU = clang_parseTranslationUnit(Index, NULL, argv, argc, 0, 0, CXTranslationUnit_None);
	CXCursor rootCursor = clang_getTranslationUnitCursor(TU);

	clang_visitChildren(rootCursor, printAST, NULL);

	clang_disposeTranslationUnit(TU);
	clang_disposeIndex(Index);
	return 0;
}
