#include "9cc.h"

// 現在着目しているトークン
Token *token;

// 入力プログラム
char *user_input;

LVar *locals;


/**
 * ノード生成関数
*/
Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

Node *new_node_lvar(LVar *lvar) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_LVAR;
    node->lvar = lvar;
    return node;
}

LVar *new_lvar() {
    LVar *lvar = calloc(1, sizeof(LVar));
    lvar->next = locals;
    lvar->name = token->str;
    lvar->len = token->len;
    lvar->offset = locals->offset + 8;
    locals = lvar;
    return lvar;
}

/**
 * ユーティリティ
*/
// 変数を名前で検索する。見つからなかった場合はNULLを返す。
LVar *find_lvar() {
  for (LVar *var = locals; var; var = var->next)
    if (var->len == token->len && !memcmp(token->str, var->name, var->len))
      return var;
  return NULL;
}


/**
 * パース関数
*/

Function *program() {
    Node head = {};
    Node *cur = &head;

    LVar lhead = {};
    lhead.offset = 0;
    locals = &lhead;

  while (!at_eof()){
    cur = cur->next = stmt();
  }
    Function *func = calloc(1, sizeof(Function));
    func->body = head.next;
    func->locals = locals->next;
    // func->stack_size = (stack_size + 15 ) / 16 * 16;
    func->stack_size = locals->offset;
    return func;
}

Node *stmt() {
  Node *node = expr();
  expect(";");
  return node;
}

Node *expr() {
  return assign();
}

Node *assign() {
  Node *node = equality();
  if (consume("=")) {
    node = new_node(ND_ASSIGN, node, assign());
  }
  return node;
}

Node *equality() {
  Node *node = relational();

  for (;;) {
    if (consume("=="))
      node = new_node(ND_EQ, node, relational());
    else if (consume("!="))
      node = new_node(ND_NEQ, node, relational());
    else
      return node;
  }
}

Node *relational() {
  Node *node = add();

  for (;;) {
    if (consume("<="))
      node = new_node(ND_LE, node, add());
    else if (consume("<"))
      node = new_node(ND_LT, node, add());
    else if (consume(">="))
      node = new_node(ND_LE, add(), node);
    else if (consume(">"))
      node = new_node(ND_LT, add(), node);
    else
      return node;
  }
}

Node *add() {
  Node *node = mul();

  for (;;) {
    if (consume("+"))
      node = new_node(ND_ADD, node, mul());
    else if (consume("-"))
      node = new_node(ND_SUB, node, mul());
    else
      return node;
  }
}

Node* mul() {
  Node *node = unary();

  for (;;) {
    if (consume("*"))
      node = new_node(ND_MUL, node, unary());
    else if (consume("/"))
      node = new_node(ND_DIV, node, unary());
    else
      return node;
  }
}

Node* unary() {
  if (consume("+"))
    return primary();
  if (consume("-"))
    return new_node(ND_SUB, new_node_num(0), primary());
  return primary();
}

Node *primary() {
  // 次のトークンが"("なら、"(" expr ")"のはず
  if (consume("(")) {
    Node *node = expr();
    expect(")");
    return node;
  }

  // 変数か
    if (consume_ident()) {
        LVar *lvar = find_lvar();
        if (!lvar) {
            lvar = new_lvar();
        }
        token = token->next;
        return new_node_lvar(lvar);
    }

  // そうでなければ数値のはず
  return new_node_num(expect_number());
}
