#include "util.h"
#include "entity.h"

//만든 맵들 중 가장 큰 사이즈의 ROW와 COL을 넣어줘야 한다.
#define MAX_MAP_ROW 13
#define MAX_MAP_COL 12

dir_type get_next_step(entity *e);
int update_stepmap(int is_enemy);
void update_stepmap_of(int enemy, int r, int c, int step);