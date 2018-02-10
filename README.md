# ConsoleGameEngine

## 사전설치 툴/라이브러리
* make
* gcc
* ncurses-dev

## 디렉토리 설명
* Game Engine : 엔진코드와 예시 클래스 하나가 정의됨
* Demo : 엔진코드와 하나 이상의 클래스로 예시 기능이 구현됨
   
## 엔진디렉토리/코드 설명
* game_engine : 게임엔진 코드 
   * data_struct : 자료구조 
      + list.c/.h : 리스트 
   * lib : 라이브러리 
      + game_engine_lib.h : 라이브러리 헤더 모음 
      + gettickcount.c : 시스템 tick을 얻음 
      + lvmv_memory.c/.h : 동적 할당 메모리 관리 라이브러리 
   + game_engine.c : 게임엔진의 main 
   + kbhit.c/.h : 간단한 키보드 체크 
   + text_graphic.c/.h : 텍스트 그래픽 관련 함수 
   + text_graphic_color.h : 텍스트 색상 정의 
* object : 클래스 정의 코드 
   + objgamemaster.c/.h : 예제 클래스(q를 입력하면 게임종료) 
   + objtext.c/.h : 예제 클래스 텍스트(지정한 색상의 텍스트가 조작키로 움직임) 
+ game_main.c : 게임 main 
