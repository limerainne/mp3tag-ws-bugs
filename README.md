# Mp3tag Tag Sources for Bugs

[![Release][release-badge]][release]

## 기능
* Bugs의 고품질 음원 태그 정보를 이용할 수 있습니다.
  * 앨범 정보, 앨범아트, 트랙 정보를 가져올 수 있습니다.
  * 가사 및 참여 아티스트 정보를 가져올 수 있습니다.
  * 동기 가사도 가져올 수 있습니다.
* 곡 제목 뒤의 설명 문구는 무시합니다. i.e. `Ah-Choo (드라마 월계수 양복점 신사들 삽입곡)` -> `Ah-Choo`
* 여러 디스크로 이루어진 경우도 가져올 수 있습니다.

## 설치

* 먼저 [Mp3tag][mp3tag-homepage] 프로그램을 설치하세요.
* 본 저장소의 [Releases][release] 페이지에서 최신 버전 소스를 받으세요.
* Windows 탐색기에서 `%appdata%\Mp3tag\data\sources\` 경로로 이동한 뒤, 받은 소스 파일 중 `src` 확장자 파일을 두세요.

## 사용법

### 앨범 정보 입력
* 앨범 단위로 곡 선택 후
* 메뉴 중 `[태그 소스] > [Bugs]`에서 원하는 기능을 골라 실행하세요.
  * 도구모음 아이콘을 이용할 수도 있습니다.
* 검색어 입력, 검색 결과 중 알맞은 앨범을 선택, 태그 정보 창에서 문구 수정, 트랙 순서 조정 순으로 진행하세요.

### 가사 정보 (+ 참여 아티스트) 입력
* 앨범 정보를 먼저 입력한 후에 사용 가능합니다.
* 입력할 곡 **한 개** 선택 후
* 같은 방식으로 기능 실행 시 Bugs 트랙 ID를 검색어로 입력받는 창이 뜹니다. 이미 표시되어 있을 겁니다.
  * 앞서 입력한 앨범 정보 중 트랙 ID도 있습니다.
* 검색 진행, 검색된 태그 정보 확인 및 입력을 진행하세요.

## 참고

* 여는 소괄호 앞에 일부러 빈 칸을 두었습니다. 싫으시면 다음 코드를 찾아 주석처리하거나 지우세요.
```
RegexpReplace "(\w)\(" "$1 (" 1
```
* 앨범 커버 해상도 기본값은 `1000`입니다. 바꾸려면 다음 코드를 찾아 `1000`을 다른 값으로 바꾸세요: `200`, `500`
```
RegexpReplace "(album/images/)(\d+)" "$1@1000"
```
* 발매년도 태그인 `YEAR`에 발매일을 전부 적고 있습니다. (`YYYY.MM.DD`) 발매년도만 적으려면
```
# 1. 아래 줄을 찾아 주석처리하거나 (앞에 '#' 글자 붙이기) 지우고
SayUntil "\">"

# 2. 아래 줄을 찾아 주석 표시를 지우세요 (앞에 붙인 '#' 글자 지우기)
# SayNChars 4
```

* 일반 가사는 `UNSYNCEDLYRICS`에, 동기 가사는 `LYRICS`에 담습니다. (삼성뮤직 호환?)

* 동기 가사: **곡 길이**가 **15분 이내**인 경우만 가져올 수 있음
  * Mp3tag 스크립트 기능 제약으로 인해 LRC 포맷 초 -> 분 변환을 수동으로 하였습니다..

[mp3tag-homepage]:https://www.mp3tag.de/en
[release]:https://github.com/limerainne/mp3tag-ws-bugs/releases
[release-badge]:https://img.shields.io/github/release/limerainne/mp3tag-ws-bugs.svg?style=for-the-badge
