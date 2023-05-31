# SPRogue

# 필요한 것
    1. resize, 없으면 설치 필요
    2. gnome-terminal
    3. gcc + make

# 환경 세팅
    1. gnome-terminal 의 preferences 로 들어가 custom font를 enable 하고 폰트 크기를 12로 지정
    2. resize가 없으면 sudo apt install resize 혹은 sudo dnf install resize 로 설치
    3. 레포를 pull
    4. 메인 디렉토리에 진입해 해당 디렉토리를 작업 디렉토리로 하여 gnome-terminal 실행
    5. gnome terminal 에서 make build 타입
    6. 이후 make run
    7. 플레이


# 조작법
    1. 방향키로 이동
    2. x 키를 눌러 examine. examine 상태에서는 우측 사이드바의 entity id를 타입하여 entity를 확인할 수 있고, 혹은 방향키를 눌러 해당하는 방향의 타일 확인 가능. '.' 키를 누르면 현재 위치한 타일 확인. 'r' 키를 누르면 현재 위치한 방의 설명 확인.
    3. q를 눌러 potato bomb 투척. 단, 보유한 폭탄이 있어야 함.
    4. w를 눌러 zombie 소환. 단, 보유한 시체가 있어야 함.
    5. e를 눌러 원거리 공격.
    6. 적에게 접근해 적이 위치한 타일로 방향키 누를시 근접공격.
    7. vial of blood, dead potato, 혹은 dead eggplant 가 위치한 타일에 오를시 해당하는 효과 발생. vial은 체력 회복, 
    dead potato는 Potato bomb 추가, dead eggplant 는 zombie 추가.
    8. 문으로 이동하여 다른 방으로 넘어갈 수 있음.


# 게임 설명
    과거 캐롯캐롯 빌리지는 당근 귀족들의 지도 아래 모든 채소가 조화롭게 살아가는 곳이었습니다. 하지만 어느날 황야에서부터 유목토끼 부족이 찾아왔고, 그들은 캐롯캐롯 빌리지를 점령하였습니다. 이제 그들은 이를 바니바니 팜이라 부르며 모든 채소를 자신들의 식량으로 삼았습니다. 그들의 폭정 아래 채소 인민들은 고통 받았습니다. 결국, 더 이상 참지 못한 이들은
    비밀결사를 결성하여 그들 스스로의 피를 바쳐 전설 속 영웅, 초-당근을 사역해 냈습니다. 이제 당신은 그들의 복수의 대리자이자 인민의 해방자로서 다시 캐롯캐롯 빌리지의 영광을 되찾아 와야 합니다.

    그를 위해 당신은 바니바니 팜의 여러 방과 층을 탐험하여 여러 적을 무찔러야 하고, 결국 가장 깊은 곳에 숨어 있는 토끼군주를 죽여야 합니다.