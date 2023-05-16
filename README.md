# SPRogue
새로운 tile 및 entity 추가하는 방법은 다음과 같습니다.

1. x_type enum에 새로운 type 추가
2. 해당하는 type에 대해 set_x_template 과 draw_x_func 함수 추가
3. init_x 함수에 해당 type 엔트리 추가