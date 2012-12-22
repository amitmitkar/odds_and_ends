#include <memory.h>
#include <clang-c/Index.h>


CXChildVisitResult printFuncHeader(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
	CXString str;
	CXCursor temp;
	CXChildVisitResult ret = CXChildVisit_Recurse;

	if (clang_getCursorKind (cursor) != CXCursor_FunctionDecl
			&& clang_getCursorKind (cursor) != CXCursor_ParmDecl) {
		ret = CXChildVisit_Continue;
		goto proc_exit;
	}

	switch (clang_getCursorKind(cursor)) {
		case CXCursor_FunctionDecl : 
			str = clang_getCursorSpelling(cursor);
			printf("/* =============================================================== *\n");
			printf(" * Name   : %s\n", clang_getCString(str));
			printf(" * Desc   :\n");
			temp = clang_getTypeDeclaration(clang_getResultType(clang_getCursorType(cursor)));
			if (clang_isInvalid(clang_getCursorKind(temp)))
				str = clang_getTypeKindSpelling(clang_getResultType(clang_getCursorType(cursor)).kind);
			else {
				str = clang_getCursorDisplayName(temp);
			}
			printf(" * Return : %s\n", clang_getCString(str));
			printf(" * ----------------------[ Params ]------------------------------- \n");
			clang_visitChildren(cursor, printFuncHeader, NULL);
			printf(" */\n");
			ret = CXChildVisit_Continue;
			break;
		case CXCursor_ParmDecl:
			str = clang_getCursorDisplayName(cursor);
			printf(" * %s :\n", clang_getCString(str));
			ret = CXChildVisit_Continue;
			break;
	}

proc_exit:
	return ret;
}

int main(int argc, char **argv)
{
	CXIndex Index = clang_createIndex(0, 0);
	CXTranslationUnit TU = clang_parseTranslationUnit(Index, NULL, argv, argc, 0, 0, CXTranslationUnit_None);
	CXCursor rootCursor = clang_getTranslationUnitCursor(TU);

	clang_visitChildren(rootCursor, printFuncHeader, NULL);

	clang_disposeTranslationUnit(TU);
	clang_disposeIndex(Index);
	return 0;
}
