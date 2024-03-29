#
# * Mp3tag web source for Bugs / Common script for parsing album page
# v0.4.12 / 230827
# 
# * Installation
# - just copy this script to:
# %appdata%\Mp3tag\data\sources\
# - NOTE '#' in filename makes submenu!

[BasedOn]=https://music.bugs.co.kr
[AlbumUrl]=https://music.bugs.co.kr/album/
[WordSeparator]=+
[Encoding]=url-utf-8

# ### Album URL example ###
# "A New Trilogy" by Lovelyz
# AlbumUrl: https://m.bugs.co.kr/album/20032445
# CoverUrl: https://image.bugsm.co.kr/album/images/<size>/200324/20032445.jpg
# -- size: 200, 500, 1000, original
# -- note: care for last 2 digit! (200324 -> 200324'45')


[ParserScriptAlbum]=...
# debug "off" "C:\Temp\Bugs\debug_album.htm"
# ###################################################################
# This script parses the albums page for all common information.
#
# The current output variable is set via the outputto command
# All these fields are used by the confirm online information dialog
# ###################################################################

## BugsAlbumID
FindLine "<meta property=\"og:url\" content=\""
OutputTo "BugsAlbumId"
FindInLine "album/"
SayUntil "\"/>"

## Album Title
FindLine "<header class=\"sectionPadding pgTitle noneLNB\">"
MoveLine +2
OutputTo "ALBUM"
FindInLine "<h1>"
SayUntil "</h1>"

## AlbumInfos
FindLine "<section class=\"sectionPadding summaryInfo summaryAlbum\">"

# cover URL
FindLine "<div class=\"photos\">"
FindLine "<img src=\""
OutputTo "CoverURL"

# TODO change cover image size
RegexpReplace "(album/images/)(\d+)" "$1@1000"
RegexpReplace "@" ""

RegexpReplace "(\w+\.\w+)\?.*" "$1"

FindInLine "<img src=\""

SayUntil "\""

# Bugs album artist ID & Album Artist name
FindLine "<table class=\"info"
FindLine "아티스트"
MoveLine +2  # "pass <td>...
Unspace
RegexpReplace "<a\s+href", "<a href"
if "<a href=\"https://music.bugs.co.kr/"
do
    # fetch artist ID and Name
    # - ID
    OutputTo "BugsAlbumArtistId"
    ifoutput "BugsAlbumArtistId"
        Say ";"
    endif
    FindInLine "artist/" 1 1
    SayRegexp "[0-9]+" "" "?"

    # - name
    MoveLine +1
    OutputTo "AlbumArtist"
    ifoutput "AlbumArtist"
        Say ", "
    endif
    # - tidy tag
    # add space before opening parenthesis
    RegexpReplace "(\w{2,})\(" "$1 ("
    # NOTE require 2+ chars. to avoid 'f(x)' -> 'f (x)'
    RegexpReplace "\" >" "\">"
    SayRest

    FindLine "</a>"
    MoveLine +2
    Unspace
while "<a href=\"https://music.bugs.co.kr/"
else
    OutputTo "AlbumArtist"
    Unspace
    SayRest
endif

# album type* ~ renamed in around 24/01/11
FindLine "유형"
MoveLine +1
OutputTo "ALBUMTYPE"
FindInLine "<td>"
SayUntil "</td>"

# genre** -- multiline
FindLine "장르"
MoveLine +2
joinuntil "</tr>"
OutputTo "GENRE"
FindInLine "\">"
SayUntil "</a>"
FindInLine "\">" 1 1
MoveChar -2
if "\">"
  do
    MoveChar +2
    Say ", "
    SayUntil "</a>"
    FindInLine "\">" 1 1
    MoveChar -2
  while "\">"
endif

# style** -- multiline
FindLine "스타일"
MoveLine +2
joinuntil "</tr>"
OutputTo "STYLE"
FindInLine "\">"
SayUntil "</a>"
FindInLine "\">" 1 1
MoveChar -2
if "\">"
  do
    MoveChar +2
    Say ", "
    SayUntil "</a>"
    FindInLine "\">" 1 1
    MoveChar -2
  while "\">"
endif

# released date*
FindLine "발매일"
MoveLine +2
OutputTo "YEAR"
FindInLine "<time datetime=\""
FindInLine ">"

# - get year.month.day
SayUntil "<"

# TODO to get 'year' only, use below
# SayNChars 4

# distributer*
FindLine "유통사" 1 1
Unspace
if "<th scope="row">유통사</th>"
  MoveLine +1
  OutputTo "DISTRIBUTER"
  FindInLine "<td>"
  SayUntil "</td>"
else
  GotoLine 1
endif

# publisher*
FindLine "기획사" 1 1
Unspace
if "<th scope="row">기획사</th>"
  MoveLine +1
  OutputTo "PUBLISHER"
  FindInLine "<td>"
  SayUntil "</td>"
else
  GotoLine 1
endif

# length*
FindLine "재생 시간"
MoveLine +1
OutputTo "ALBUMLENGTH"
FindInLine "<time datetime=\""
SayUntil "\">"

FindLine "<h1>수록곡</h1>"
MoveLine +1
OutputTo "TotalTracks"
FindInLine "\"desc\">("
SayUntil ")</p>"

# -------
FindLine "\"list trackList"

# if multiple disc
FindLine "<tr class=\"cd\">" 1 1
Unspace
if "<tr class=\"cd\">"
  set "tCD" "On"
endif

GotoLine 1
FindLine "\"list trackList"

Do
  FindLine "<tr albumId=\""
  
#  OutputTo "Tracks"
#  SayOutput "TotalTracks"
#  Say "|"
  
  # track Id
  FindLine "<input type=\"checkbox\""
  OutputTo "BugsTrackId"
  FindInLine "value=\""
  SayUntil "\""
  # SayRegexp "[0-9]{2,}"
  Say "|"
  
  # disc id if
  ifoutput "tCD"
    OutputTo "DISCNUMBER"
    FindInLine "disc_id=\""
    SayUntil "\""
    Say "|"
  endif

  # track title
  # remove unnecessary tags
  RegexpReplace "\(.+곡\)" ""
  RegexpReplace " \">" "\">"
  
  OutputTo "tTITLE"
  FindInLine "title=\""
  SayUntil "\""
  Say "|"
  
  # track number
  FindLine "<p class=\"trackIndex\">"
  MoveLine +1
  OutputTo "tTrack"
  FindInLine "<em>"
  SayUntil "</em>"
  Say "|"
  
  # is title?
  MoveLine +1
  OutputTo "TitleTrack"
  if "<span class=\"albumTitle\""
    Say "O"
  endif
  Say "|"
  
  # track artist
  FindLine "<p class=\"artist\""
  FindLine "<a"

  # if multiple artists
  MoveLine +2
  Unspace
  if "<a "   # multiple artist!
    # - pick only relevant information
    RegexpReplace ".*PopLayer\(this, '(.+)', ''\);.*", "$1"
    
    OutputTo "BugsArtistId"
    Replace "\\\\n" "$n$"
    RegexpReplace ".+?\|\|.+?\|\|(\d+)\|\|OK(\$n\$)?" "$1$2"
    Replace "$n$" ";"
    SayRest
    Say "|"

    MoveLine -1
    MoveLine +1
    RegexpReplace ".*PopLayer\(this, '(.+)', ''\);.*", "$1"

    OutputTo "tARTIST"
    Replace "\\\\n" "$n$"
    RegexpReplace ".+?\|\|(.+?)\|\|\d+\|\|OK(\$n\$)?" "$1$2"
    Replace "$n$" ", "
    # add space before opening parenthesis
    RegexpReplace "(\w{2,})\(" "$1 ("
    # NOTE require 2+ chars. to avoid 'f(x)' -> 'f (x)'
    SayRest
    Say "|"

  else
    MoveLine -2

    # - track artist Id
    OutputTo "BugsArtistId"
    FindInLine "artist/" 1 1
    SayRegexp "[0-9]+" "" "?"
    Say "|"

    # find artist string
    FindLine "\">"
    
    # - tidy DUPLICATED artist tag
    # : add space before opening parenthesis
    RegexpReplace "(\w{2,})\(" "$1 ("
    # NOTE require 2+ chars. to avoid 'f(x)' -> 'f (x)'
    
    OutputTo "tARTIST"
    FindInLine "\">"
    SayUntilML "</a>"
    Say "|"
  endif
  
  OutputTo "ttracks"
  Say "|"

  # FindLine "</tr>"
  # MoveLine +1

  FindLine "<tr albumId=\"" 1 1
While "<tr albumId=\""

# -- comment --
GotoLine 1
FindLine "<section class=\"sectionPadding contents albumDesc\">" 1 1
Unspace
if "<section class=\"sectionPadding contents albumDesc\">"
  FindLine "<p id=\"albumContents\""
  MoveLine +1
  OutputTo "ALBUMDESCRIPTION"
  # NOTE CR+LF, not just LF, it's Windows...!
  RegexpReplace "<br/>" "\r\n"
  RegexpReplace "</?\w+>" ""
  SayRest
endif

OutputTo "ARTIST"
SayOutput "tARTIST"
OutputTo "TITLE"
SayOutput "tTITLE"
OutputTo "track"
SayOutput "ttrack"
OutputTo "tracks"
SayOutput "ttracks"
# NOTE to show multiple tracks, last item should not be an empty one or a single item

# drop temporary variables
set "tARTIST"
set "tTITLE"
set "ttrack"
set "ttracks"
set "tCD"
