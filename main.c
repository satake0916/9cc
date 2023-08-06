#include "9cc.h"

int main(int argc, char **argv) {
  // printf("start\n");
  if (argc != 2) {
    error_at(token->str, "引数の個数が正しくありません");
    return 1;
  }

  // トークナイズする
  // printf("tokenize\n");
  tokenize(argv[1]);

  // パースする
  // printf("parsing\n");
  Function *func = program();

  // 出力する
  // printf("generating code\n");
  codegen(func);

  return 0;
}
