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

int GetToken();
struct StatementNode* parse_generate_intermediate_representation();
struct StatementNode* parse_body();
struct StatementNode* parse_stmt_list();
struct StatementNode* parse_stmt();


string Token = "";
map<string, ValueNode*> variableMap;

struct StatementNode* parse_generate_intermediate_representation()
{
    struct StatementNode* program;
//    program = new StatementNode;
//    program->type = NOOP_STMT;
//    struct StatementNode* st = new StatementNode;
//    program->next = st;
//    st->next = NULL;
//    struct StatementNode* temp = st;
    
    GetToken();
    while (ttype != SEMICOLON || ttype == COMMA)
    {
        if (ttype == COMMA)
        {
            GetToken();
        }
        if (ttype == ID)
        {
            variableMap[Token] = new ValueNode;
            char* ptr = new char[MAX_TOKEN_LENGTH];
            sprintf(ptr,"%.4s", Token.c_str());
            variableMap[Token]->name = ptr;
            variableMap[Token]->value = 0;
//            temp = new StatementNode;
//            temp->next = new StatementNode;
//            temp->type = ASSIGN_STMT;
//            temp->assign_stmt = new AssignmentStatement;
//            temp->assign_stmt->left_hand_side->name = ptr;
//            temp->assign_stmt->op = 0;
//            temp->assign_stmt->operand1 = 0;
//            temp = temp->next;
            GetToken();
        }
    }
    program = parse_body();
    return program;
};

struct StatementNode* parse_stmt()
{
    struct StatementNode* st = new StatementNode;
    int type = ttype;
    string tokenTemp = token;
    if (ttype == ID)
    {
        st->type = ASSIGN_STMT;
        struct AssignmentStatement* assignment_node = new AssignmentStatement;
        st->assign_stmt = assignment_node;
        
        assignment_node->left_hand_side = variableMap[Token];
        GetToken();
        if (ttype == EQUAL)
        {
            GetToken();
            if (ttype == ID)
            {
                assignment_node->operand1 = variableMap[Token];
                GetToken();
                if (ttype == PLUS || ttype == MINUS || ttype == MULT || ttype == DIV)
                {
                    assignment_node->op = ttype;
                    GetToken();
                    if (ttype == ID)
                    {
                        assignment_node->operand2 = variableMap[Token];
                        GetToken();
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
                        GetToken();
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
                GetToken();
                if (ttype == PLUS || ttype == MINUS || ttype == MULT || ttype == DIV)
                {
                    assignment_node->op = ttype;
                    GetToken();
                    if (ttype == ID)
                    {
                        assignment_node->operand2 = variableMap[Token];
                        GetToken();
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
                        GetToken();
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
    }
    else if (ttype == IF)
    {
        st->type = IF_STMT;
        struct IfStatement* if_node = new IfStatement;
        st->if_stmt = if_node;
        
        GetToken();
        if (ttype == ID)
        {
            if_node->condition_operand1 = variableMap[Token];
            GetToken();
            if (ttype == GREATER || ttype == LESS || ttype == NOTEQUAL)
            {
                if_node->condition_op = ttype;
                GetToken();
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
            GetToken();
            if (ttype == GREATER || ttype == LESS || ttype == NOTEQUAL)
            {
                if_node->condition_op = ttype;
                GetToken();
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
    }
    else if (ttype == PRINT)
    {
        st->type = PRINT_STMT;
        struct PrintStatement* print_node = new PrintStatement;
        st->print_stmt = print_node;
        GetToken();
        if (ttype == ID)
        {
            print_node->id = variableMap[Token];
            GetToken();
            if (ttype == SEMICOLON)
            {
                return st;
            }
        }
    }
    else if (ttype == WHILE)
    {
        st->type = IF_STMT;
        struct IfStatement* if_node = new IfStatement;
        st->if_stmt = if_node;
        
        GetToken();
        if (ttype == ID)
        {
            if_node->condition_operand1 = variableMap[Token];
            GetToken();
            if (ttype == GREATER || ttype == LESS || ttype == NOTEQUAL)
            {
                if_node->condition_op = ttype;
                GetToken();
                if (ttype == ID)
                {
                    if_node->condition_operand2 = variableMap[Token];
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
            GetToken();
            if (ttype == GREATER || ttype == LESS || ttype == NOTEQUAL)
            {
                if_node->condition_op = ttype;
                GetToken();
                if (ttype == ID)
                {
                    if_node->condition_operand2 = variableMap[Token];
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
    }
    else if (ttype == SWITCH)
    {
        
    }
    return NULL;
};

struct StatementNode* parse_stmt_list()
{
    struct StatementNode* st;
    struct StatementNode* stl;
    
    st = parse_stmt();
    GetToken();
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

struct StatementNode* parse_body()
{
    struct StatementNode* stl;
    GetToken();
    if (ttype == LBRACE)
    {
        GetToken();
        stl = parse_stmt_list();
        GetToken();
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

int GetToken()
{
    int type = getToken();
    Token = token;
    return type;
};