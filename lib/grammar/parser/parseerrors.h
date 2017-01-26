//
// Created by bknun on 1/6/2017.
//

#ifndef SHARP_PARSEERRORS_H
#define SHARP_PARSEERRORS_H

#include <list>
#include "../../../stdimports.h"
#include "../../util/keypair.h"
#include "tokenizer/tokenentity.h"

enum p_errors
{
    UNEXPECTED_SYMBOL = 0,
    ILLEGAL_NUMBER_FORMAT = 1,
    UNEXPECTED_EOF = 2,
    EXPECTED_STRING_LITERAL_EOF = 3,
    ILLEGAL_STRING_FORMAT = 4,
    EXPECTED_CHAR_LITERAL_EOF = 5,
    ILLEGAL_CHAR_LITERAL_FORMAT = 6,
    GENERIC = 7,
    ILLEGAL_ACCESS_DECLARATION = 8,
    ILLEGAL_BRACKET_MISMATCH = 9,
    MISSING_BRACKET = 10,

    NO_ERR = 999
};

static list<keypair<p_errors, string>> predefined_errs;
void int_errs();

struct parseerror
{
public:
    parseerror()
    {
        id = NO_ERR;
        error = ' ';
        line = 0;
        col = 0;
    }

    parseerror(keypair<p_errors, string> err, int l, int c, string addon = "")
    {
        id = err.key;
        error = (err.value + addon);
        line = l;
        col = c;
    }

    parseerror(keypair<p_errors, string> err, token_entity token, string addon = "")
    {
        id = err.key;
        error = (err.value + addon);
        line = token.getline();
        col = token.getcolumn();
    }

    p_errors id;
    string error;
    int line;
    int col;
};

class ast;

class Errors
{
public:
    Errors(list<string>* lines)
    :
            lines(lines),
            teCursor(-1)
    {
        errors = new list<parseerror>();
        uo_errors = new list<parseerror>();
        _testerrors = new list<list<parseerror>*>();
        lasterr = parseerror();
        lastcheckederr = parseerror();
    }

    string getall_errors();
    string getuo_errors();
    uint64_t error_count() { return errors->size(); }
    uint64_t uoerror_count() { return uo_errors->size(); }
    int newerror(p_errors err, token_entity token, string xcmts = "");
    void newerror(p_errors err, int l, int c, string xcmts = "");
    bool _errs();
    void enablecheck_mode();
    void fail();
    void pass();

    void free();

private:
    string getline(int line);
    keypair<p_errors, string> geterrorbyid(p_errors);
    list<parseerror>* gettesterrorlist();
    void addtesterror_list();
    void removetesterror_list();

    list<string>* lines;
    list<parseerror>* errors, *uo_errors;
    list<list<parseerror>*>* _testerrors;
    int64_t  teCursor;
    parseerror lasterr;
    parseerror lastcheckederr;
    bool _err, cm;

    bool shouldreport(token_entity *token, const parseerror &last_err, const parseerror &e) const;

    string geterrors(list<parseerror> *errors);
};

#endif //SHARP_PARSEERRORS_H
