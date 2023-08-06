#include "9cc.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    error_at(token->str, "引数の個数が正しくありません");
    return 1;
  }

  // 入力を保存する
  user_input = argv[1];

  // トークナイズする
  token = tokenize();

  // パースする
  Node *node = expr();

  // アセンブリの前半部分を出力
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");

  // 抽象構文木をおりながらコード生成
  gen(node);

  // RAXに値をロードしてからリターンする
  printf("  pop rax\n");
  printf("  ret\n");
  return 0;
}
