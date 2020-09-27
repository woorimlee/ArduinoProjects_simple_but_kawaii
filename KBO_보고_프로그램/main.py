from urllib import request
from bs4 import BeautifulSoup
import datetime
import refresh_access_token as rat
import post_kakao_message as pkm

if __name__ == "__main__":

    #1. 오늘 날짜 기반으로 URL 만들기
    today = datetime.datetime.today()
    if today.day < 10 : #일
        day = "0%d" % (today.day)
    else :
        day = "%d" % (today.day)
    if today.month < 10:#월
        month = "0%d" %(today.month)
    else:
        month = "%d" %(today.month)
    year = str(today.year)#년
    url = "https://sports.news.naver.com/kbaseball/schedule/index.nhn?date="+ year+month+day +"&month="+ month + "&year=" + year+"&teamCode="

    #2. HTML Parsing
    html = request.urlopen(url).read()
    soup = BeautifulSoup(html, 'html.parser')
    #홀수와 짝수 날 크롤링해야 할 태그가 다름. 
    sch = soup.select("div.sch_tb.selected")
    sch2 = soup.select("div.sch_tb2.selected")

    flag = True
    #3. 경기 시간, 경기 팀, 경기 결과(문자 중계), 경기 영상(TV 중계) 출력하기
    if sch : 
        all_games = sch[0].select('table > tbody > tr')#경기 취소 확인    
    elif sch2 :
        all_games = sch2[0].select('table > tbody > tr')#경기 취소 확인
    else : 
        print("오늘은 프로 야구 경기가 없습니다.")
        flag = False

    message_data = {"home" : [], 
                   "away" : [],
                   "score" : [],
                   "time" : [],
                   "link" : []}

    if(flag) :
        #다섯 번의 경기가 있음
        for i in range(5) :
            #총 세 가지 경우를 판단해야함.
            #1) 경기 취소 여부
            #2) 문자 중계 < - > 경기 결과 중 하나
            #3) 경기 영상 올라옴 < - > 올라오지 않음 중 하나

            game_cancel = all_games[i].select('span.suspended')
            lft = all_games[i].select('span.team_lft') #왼쪽 팀
            rgt = all_games[i].select('span.team_rgt') #오른쪽 팀
            hour = all_games[i].select('span.td_hour') #경기 시간
            score = all_games[i].select('strong.td_score') #경기 기록 or VS
            print(hour[0].string,lft[0].string, score[0].text, rgt[0].string)
            message_data["time"].append(hour[0].string)
            message_data["home"].append(lft[0].string)
            message_data["away"].append(rgt[0].string)
            message_data["score"].append(score[0].text)

            #1) 경기 취소 여부 검사
            if game_cancel : 
                #경기가 취소된 경우
                print(game_cancel[0].string)
            else : 
                #경기가 진행되는 경우
                #경기가 진행중이면 a 태그가 두 개 나오고, (문자 중계, TV 중계)
                #진행중이지 않으면 a 태그가 하나만 나온다. (문자 중계)
                game_info_link = all_games[i].select('td > span > a')
                if len(game_info_link) == 2 :
                    for a in game_info_link :
                        img = a.select('img')
                        link_url = "https://sports.news.naver.com/" + a['href']
                        print(img[0]['alt'], ":", link_url)
                        if "textRelay" in link_url :
                            message_data["link"].append(link_url)
                elif len(game_info_link) == 1 :
                    img = game_info_link[0].select('img')
                    link_url = "https://sports.news.naver.com/" + game_info_link[0]['href']
                    print("아직 경기가 시작되지 않아, TV 중계 링크는 없습니다.")
                    print(img[0]['alt'], ":", link_url)
                else : 
                    print("에러")
            print()

    rat.get_access_token()
    pkm.post_message(message_data, year+month+day)
