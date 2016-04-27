//
//  structure.cpp
//  Project5
//
//  Created by Frank Palmisano on 4/25/16.
//  Copyright © 2016 Frank Palmisano. All rights reserved.
//

extern "C" {
#include "compiler.h"
}

#include <map>
#include <string>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int GetTokenString();
struct StatementNode* parse_generate_intermediate_representation();
struct StatementNode* parse_body();
struct StatementNode* parse_stmt_list();
struct StatementNode* parse_stmt();
struct StatementNode* parse_case_list();
struct StatementNode* parse_case();
struct StatementNode* parse_switch_body();

string Token;
string varID = "";


map<string, ValueNode*> variableMap;

struct StatementNode* parse_generate_intermediate_representation()
{
    struct StatementNode* program;
    
    GetTokenString();
    while (ttype != SEMICOLON || ttype == COMMA)
    {
        if (ttype == COMMA)
        {
            GetTokenString();
        }
        if (ttype == ID)
        {
            variableMap[Token] = new ValueNode;
            char* ptr = new char[MAX_TOKEN_LENGTH];
            sprintf(ptr,"%.4s", Token.c_str());
            variableMap[Token]->name = ptr;
            variableMap[Token]->value = 0;
            GetTokenString();
        }
    }
    program = parse_body();
    return program;
};

struct StatementNode* parse_stmt()
{
    struct StatementNode* st = new StatementNode;
    st->next = NULL;
    switch (ttype)
    {
        case ID:
        {
            st->type = ASSIGN_STMT;
            struct AssignmentStatement* assignment_node = new AssignmentStatement;
            st->assign_stmt = assignment_node;
            
            assignment_node->left_hand_side = variableMap[Token];
            GetTokenString();
            if (ttype == EQUAL)
            {
                GetTokenString();
                if (ttype == ID)
                {
                    assignment_node->operand1 = variableMap[Token];
                    GetTokenString();
                    if (ttype == PLUS || ttype == MINUS || ttype == MULT || ttype == DIV)
                    {
                        assignment_node->op = ttype;
                        GetTokenString();
                        if (ttype == ID)
                        {
                            assignment_node->operand2 = variableMap[Token];
                            GetTokenString();
                            if (ttype == SEMICOLON)
                            {
                                return st;
                            }
                            else
                            {
                                debug("Expecting SEMICOLON while parsing assignment statement.");
                                exit(1);
                            }
                        }
                        else if (ttype == NUM)
                        {
                            assignment_node->operand2 = new ValueNode;
                            assignment_node->operand2->value = atoi(token);
                            GetTokenString();
                            if (ttype == SEMICOLON)
                            {
                                return st;
                            }
                            else
                            {
                                debug("Expecting SEMICOLON while parsing assignment statement.");
                                exit(1);
                            }
                        }
                        else
                        {
                            debug("Expecting ID or NUM while parsing operand 2 of assignment statement.");
                            exit(1);
                        }
                    }
                    else if (ttype == SEMICOLON)
                    {
                        assignment_node->op = 0;
                        return st;
                    }
                    else
                    {
                        debug("Expecting operand type after parsing operand1 of assignment statement.");
                        exit(1);
                    }
                }
                else if (ttype == NUM)
                {
                    assignment_node->operand1 = new ValueNode;
                    assignment_node->operand1->value = atoi(token);
                    GetTokenString();
                    if (ttype == PLUS || ttype == MINUS || ttype == MULT || ttype == DIV)
                    {
                        assignment_node->op = ttype;
                        GetTokenString();
                        if (ttype == ID)
                        {
                            assignment_node->operand2 = variableMap[Token];
                            GetTokenString();
                            if (ttype == SEMICOLON)
                            {
                                return st;
                            }
                            else
                            {
                                debug("Expecting SEMICOLON while parsing assignment statement.");
                                exit(1);
                            }
                            
                        }
                        else if (ttype == NUM)
                        {
                            assignment_node->operand2 = new ValueNode;
                            assignment_node->operand2->value = atoi(token);
                            GetTokenString();
                            if (ttype == SEMICOLON)
                            {
                                return st;
                            }
                            else
                            {
                                debug("Expecting SEMICOLON while parsing assignment statement.");
                                exit(1);
                            }
                        }
                        else
                        {
                            debug("Expecting ID or NUM while parsing operand 2 of assignment statement.");
                            exit(1);
                        }
                    }
                    else if (ttype == SEMICOLON)
                    {
                        assignment_node->op = 0;
                        return st;
                    }
                    else
                    {
                        debug("Expecting operand type after parsing operand1 of assignment statement.");
                        exit(1);
                    }
                }
                else
                {
                    debug("Expecting ID or NUM while parsing assignment statement.");
                    exit(1);
                }
            }
            else
            {
                debug("Expecting EQUAL while parsing Assignment statement.");
                exit(1);
            }
            break;
        }
        case IF:
        {
            st->type = IF_STMT;
            struct IfStatement* if_node = new IfStatement;
            st->if_stmt = if_node;
            
            GetTokenString();
            if (ttype == ID)
            {
                if_node->condition_operand1 = variableMap[Token];
                GetTokenString();
                if (ttype == GREATER || ttype == LESS || ttype == NOTEQUAL)
                {
                    if_node->condition_op = ttype;
                    GetTokenString();
                    if (ttype == ID)
                    {
                        if_node->condition_operand2 = variableMap[Token];
                        if_node->true_branch = parse_body();
                        struct StatementNode* no_node = new StatementNode;
                        no_node->type = NOOP_STMT;
                        no_node->next = NULL;
                        struct StatementNode* it = if_node->true_branch;
                        while (it->next != NULL)
                        {
                            it = it->next;
                        }
                        it->next = no_node;
                        if_node->false_branch = no_node;
                        st->next = no_node;
                        return st;
                    }
                    else if (ttype == NUM)
                    {
                        if_node->condition_operand2 = new ValueNode;
                        if_node->condition_operand2->value = atoi(token);
                        if_node->true_branch = parse_body();
                        struct StatementNode* no_node = new StatementNode;
                        no_node->type = NOOP_STMT;
                        no_node->next = NULL;
                        struct StatementNode* it = if_node->true_branch;
                        while (it->next != NULL)
                        {
                            it = it->next;
                        }
                        it->next = no_node;
                        if_node->false_branch = no_node;
                        st->next = no_node;
                        return st;
                    }
                }
                else
                {
                    debug("Condition OP expected while parsing if statment.");
                    exit(1);
                }
            }
            else if (ttype == NUM)
            {
                if_node->condition_operand1 = new ValueNode;
                if_node->condition_operand1->value = atoi(token);
                GetTokenString();
                if (ttype == GREATER || ttype == LESS || ttype == NOTEQUAL)
                {
                    if_node->condition_op = ttype;
                    GetTokenString();
                    if (ttype == ID)
                    {
                        if_node->condition_operand2 = variableMap[Token];
                        if_node->true_branch = parse_body();
                        struct StatementNode* no_node = new StatementNode;
                        no_node->type = NOOP_STMT;
                        no_node->next = NULL;
                        struct StatementNode* it = if_node->true_branch;
                        while (it->next != NULL)
                        {
                            it = it->next;
                        }
                        it->next = no_node;
                        if_node->false_branch = no_node;
                        st->next = no_node;
                        return st;
                    }
                    else if (ttype == NUM)
                    {
                        if_node->condition_operand2 = new ValueNode;
                        if_node->condition_operand2->value = atoi(token);
                        if_node->true_branch = parse_body();
                        struct StatementNode* no_node = new StatementNode;
                        no_node->type = NOOP_STMT;
                        no_node->next = NULL;
                        struct StatementNode* it = if_node->true_branch;
                        while (it->next != NULL)
                        {
                            it = it->next;
                        }
                        it->next = no_node;
                        if_node->false_branch = no_node;
                        st->next = no_node;
                        return st;
                    }
                }
                else
                {
                    debug("Condition OP expected while parsing if statment.");
                    exit(1);
                }
            }
            break;
        }
        case PRINT:
        {
            st->type = PRINT_STMT;
            struct PrintStatement* print_node = new PrintStatement;
            st->print_stmt = print_node;
            GetTokenString();
            if (ttype == ID)
            {
                print_node->id = variableMap[Token];
                GetTokenString();
                if (ttype == SEMICOLON)
                {
                    return st;
                }
            }
            break;
        }
        case WHILE:
        {
            st->type = IF_STMT;
            struct IfStatement* if_node = new IfStatement;
            
            st->if_stmt = if_node;
            
            GetTokenString();
            if (ttype == ID)
            {
                if_node->condition_operand1 = variableMap[Token];
                GetTokenString();
                if (ttype == GREATER || ttype == LESS || ttype == NOTEQUAL)
                {
                    if_node->condition_op = ttype;
                    GetTokenString();
                    if (ttype == ID)
                    {
                        if_node->condition_operand2 = variableMap[Token];
                        if_node->true_branch = parse_body();
                        struct StatementNode* gt = new StatementNode;
                        gt->next = NULL;
                        gt->type = GOTO_STMT;
                        struct GotoStatement* goto_node = new GotoStatement;
                        gt->goto_stmt = goto_node;
                        goto_node->target = st;
                        struct StatementNode* it = if_node->true_branch;
                        while (it->next != NULL)
                        {
                            it = it->next;
                        }
                        it->next = gt;
                        struct StatementNode* no_node = new StatementNode;
                        no_node->type = NOOP_STMT;
                        no_node->next = NULL;
                        if_node->false_branch = no_node;
                        st->next = no_node;
                        return st;
                    }
                    else if (ttype == NUM)
                    {
                        if_node->condition_operand2 = new ValueNode;
                        if_node->condition_operand2->value = atoi(token);
                        if_node->true_branch = parse_body();
                        struct StatementNode* gt = new StatementNode;
                        gt->type = GOTO_STMT;
                        struct GotoStatement* goto_node = new GotoStatement;
                        gt->goto_stmt = goto_node;
                        goto_node->target = st;
                        struct StatementNode* it = if_node->true_branch;
                        while (it->next != NULL)
                        {
                            it = it->next;
                        }
                        it->next = gt;
                        struct StatementNode* no_node = new StatementNode;
                        no_node->type = NOOP_STMT;
                        no_node->next = NULL;
                        if_node->false_branch = no_node;
                        st->next = no_node;
                        return st;
                    }
                }
                else
                {
                    debug("Condition OP expected while parsing if statment.");
                    exit(1);
                }
            }
            else if (ttype == NUM)
            {
                if_node->condition_operand1 = new ValueNode;
                if_node->condition_operand1->value = atoi(token);
                GetTokenString();
                if (ttype == GREATER || ttype == LESS || ttype == NOTEQUAL)
                {
                    if_node->condition_op = ttype;
                    GetTokenString();
                    if (ttype == ID)
                    {
                        if_node->condition_operand2 = variableMap[Token];
                        if_node->true_branch = parse_body();
                        struct StatementNode* gt = new StatementNode;
                        gt->next = NULL;
                        gt->type = GOTO_STMT;
                        struct GotoStatement* goto_node = new GotoStatement;
                        gt->goto_stmt = goto_node;
                        goto_node->target = st;
                        struct StatementNode* it = if_node->true_branch;
                        while (it->next != NULL)
                        {
                            it = it->next;
                        }
                        it->next = gt;
                        struct StatementNode* no_node = new StatementNode;
                        no_node->type = NOOP_STMT;
                        no_node->next = NULL;
                        if_node->false_branch = no_node;
                        st->next = no_node;
                        return st;
                    }
                    else if (ttype == NUM)
                    {
                        if_node->condition_operand2 = new ValueNode;
                        if_node->condition_operand2->value = atoi(token);
                        if_node->true_branch = parse_body();
                        struct StatementNode* gt = new StatementNode;
                        gt->next = NULL;
                        gt->type = GOTO_STMT;
                        struct GotoStatement* goto_node = new GotoStatement;
                        gt->goto_stmt = goto_node;
                        goto_node->target = st;
                        struct StatementNode* it = if_node->true_branch;
                        while (it->next != NULL)
                        {
                            it = it->next;
                        }
                        it->next = gt;
                        struct StatementNode* no_node = new StatementNode;
                        no_node->type = NOOP_STMT;
                        no_node->next = NULL;
                        if_node->false_branch = no_node;
                        st->next = no_node;
                        return st;
                    }
                }
                else
                {
                    debug("Condition OP expected while parsing if statment.");
                    exit(1);
                }
            }
            break;
        }
        case SWITCH:
        {
            GetTokenString();
            if (ttype == ID)
            {
                varID = Token;
                st = parse_switch_body();
                return st;
            }
            else
            {
                debug("Expecting initial ID while parsing Switch statement.");
                exit(1);
            }
            break;
        }
        default:
        {
            debug("No.");
            exit(1);
            break;
        }
    }
    return NULL;
};

struct StatementNode* parse_case()
{
    struct StatementNode* st = new StatementNode;
    st->next = NULL;
    
    if (ttype == CASE)
    {
        st->type = IF_STMT;
        struct IfStatement* if_node = new IfStatement;
        st->if_stmt = if_node;
        GetTokenString();
        if (ttype == NUM)
        {
            if_node->condition_operand1 = variableMap[varID];
            if_node->condition_op = NOTEQUAL;
            if_node->condition_operand2 = new ValueNode;
            if_node->condition_operand2->value = atoi(token);
            GetTokenString();
            if (ttype == COLON)
            {
                if_node->false_branch = parse_body();
                struct StatementNode* it = if_node->false_branch;
                while (it->next != NULL)
                {
                    it = it->next;
                }
                struct StatementNode* no_node = new StatementNode;
                no_node->type = NOOP_STMT;
                no_node->next = NULL;
                it->next = no_node;
                if_node->true_branch = no_node;
                st->next = no_node;
                return st;
            }
        }
        else
        {
            debug("Expecting COLON while parsing case.");
            exit(1);
        }
    }
    else if (ttype == DEFAULT)
    {
        st->type = GOTO_STMT;
        struct GotoStatement* gt = new GotoStatement;
        st->goto_stmt = gt;
        GetTokenString();
        if (ttype == COLON)
        {
            gt->target = parse_body();
            struct StatementNode* it = gt->target;
            while (it->next != NULL)
            {
                it = it->next;
            }
            struct StatementNode* no_node = new StatementNode;
            no_node->type = NOOP_STMT;
            no_node->next = NULL;
            it->next = no_node;
            st->next = no_node;
            return st;
        }
        else
        {
            debug("Expecting COLON while parsing case DEFAULT.");
            exit(1);
        }
    }
    else
    {
        debug("Expecting CASE or DEFAULT while parsing case.");
        exit(1);
    }
    return NULL;
};

struct StatementNode* parse_case_list()
{
    struct StatementNode* st;
    struct StatementNode* stl;
    
    st = parse_case();
    GetTokenString();
    if (ttype == CASE || ttype == DEFAULT)
    {
        stl = parse_case_list();
        
        if (st->type == IF_STMT)
        {
            st->next->next = stl;
        }
        else if (st->type == NOOP_STMT)
        {
            st->next = stl;
        }
        return st;
    }
    else
    {
        ungetToken();
        return st;
    }
};

struct StatementNode* parse_stmt_list()
{
    struct StatementNode* st;
    struct StatementNode* stl;
    
    st = parse_stmt();
    GetTokenString();
    if (ttype == ID || ttype == PRINT || ttype == IF || ttype == WHILE || ttype == SWITCH)
    {
        stl = parse_stmt_list();
        
        if (st->type == IF_STMT)
        {
            st->next->next = stl;
        }
        else
        {
            st->next = stl;
        }
        return st;
    }
    else
    {
        ungetToken();
        return st;
    }
};

struct StatementNode* parse_switch_body()
{
    struct StatementNode* stl;
    
    GetTokenString();
    if (ttype == LBRACE)
    {
        GetTokenString();
        stl = parse_case_list();
        GetTokenString();
        if (ttype == RBRACE)
        {
            return stl;
        }
        else
        {
            debug("Expecting RBRACE while parsing switch body.");
            exit(1);
        }
    }
    else
    {
        debug("Expecting LBRACE while parsing switch body.");
        exit(1);
    }
    return NULL;
};
struct StatementNode* parse_body()
{
    struct StatementNode* stl;
    GetTokenString();
    if (ttype == LBRACE)
    {
        GetTokenString();
        stl = parse_stmt_list();
        GetTokenString();
        if (ttype == RBRACE)
        {
            return stl;
        }
        else
        {
            debug("Expecting RBRACE while parsing body.");
            exit(1);
        }
    }
    else
    {
        debug("Expecting LBRACE while parsing body.");
        exit(1);
    }
    return NULL;
};

int GetTokenString()
{
    int type = getToken();
    Token = string(token);
    return type;
};