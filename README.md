# Adaptive Cruise Control 

- This project is about ACC(Adaptive Cruise Control) at Hyundai Autoever Embedded School 2nd
- We made ACC system for SDV with using some hardwares

## Members
- [Dongjin Sun](https://github.com/DongjinSun)
- [yejin](https://github.com/yejin826)
- [obsidian0402](https://github.com/obsidian0402)

## Hardware
- [Infineon Easy board TC275](https://www.digikey.kr/ko/products/detail/infineon-technologies/KITAURIXTC275ARDSBTOBO1/10667578?utm_adgroup=&utm_source=google&utm_medium=cpc&utm_campaign=Pmax%20Shopping_Product_All&utm_term=&utm_content=&utm_id=go_cmp-21036528233_adg-_ad-__dev-c_ext-_prd-10667578_sig-Cj0KCQjw-ai0BhDPARIsAB6hmP4GtyE3AhTDe7vOSXGbpmWHTTiVLdla1R_-MY38erItiDZqgorse6UaAobREALw_wcB&gad_source=1&gclid=Cj0KCQjw-ai0BhDPARIsAB6hmP4GtyE3AhTDe7vOSXGbpmWHTTiVLdla1R_-MY38erItiDZqgorse6UaAobREALw_wcB)
- [Ultrasonic Sensor HC-SR04](https://eduino.kr/product/detail.html?product_no=39&nbsrc=adwords_x&nbkw=&gad_source=1&gclid=Cj0KCQjw-ai0BhDPARIsAB6hmP6dU0ZLRg7IgXNazApL4weh4fc3JwsXorq5QciUlTTf7ksCfmAgqcgaAlNrEALw_wcB)
- [Arduino Motor Shield](https://www.icbanq.com/P006971500?utm_source=google&utm_medium=cpc&utm_campaign=%EC%87%BC%ED%95%91_%EC%8B%A4%EC%A0%81%EC%B5%9C%EB%8C%80%ED%99%94%EC%BD%94%EB%94%A9&utm_id=%EC%87%BC%ED%95%91_%EC%8B%A4%EC%A0%81%EC%B5%9C%EB%8C%80%ED%99%94%EC%BD%94%EB%94%A9&utm_term=notset&utm_content=%EC%87%BC%ED%95%91_%EC%8B%A4%EC%A0%81%EC%B5%9C%EB%8C%80%ED%99%94%EC%BD%94%EB%94%A9&gad_source=1&gclid=Cj0KCQjw-ai0BhDPARIsAB6hmP7On3Kbtp7nBxEvKo8yfjq5UdLU4JgkGMIDSJbsLtsWIXCcQlugK2EaAtTTEALw_wcB)
- [Motor](https://m.motorbank.kr/goods/goods_view.php?goodsNo=1000009681)

## Tools
- [Aurix Studio](https://www.infineon.com/cms/en/tools/aurix-tools/free-tools/infineon/)
- [TeraTerm](https://teratermproject.github.io/index-en.html)

## ACC functions

- Using ultrasonic sensor for obstacle detection and avoidance

- Path smoothing using Bezier curves

- Optimizing path parameters using Newton-Raphson method
  
- Precise position estimation using encoders


## System Architecture
<br></br>
![시스템아키텍쳐](https://github.com/NahyunEE/AdaptiveCruiseControl/assets/50420981/41f3c79a-603e-45bb-bb33-ace6cc248e40)


## Software Architecture
<br></br>
![소프트웨어아키텍쳐](https://github.com/NahyunEE/AdaptiveCruiseControl/assets/50420981/7df23aba-2fdf-468a-a3f7-c4782f3c8469)


## Hardware Architecture
<br></br>
![하드웨어아키텍쳐](https://github.com/NahyunEE/AdaptiveCruiseControl/assets/50420981/8af89414-2306-41ec-ab2c-89e4b7637491)

## Key Points

### Low Pass Filter
- Low Pass Filter for filtering noise from  UltraSonic Sensors
- Based on the formula below
 <br></br>
![LowPassFilter](https://github.com/NahyunEE/AdaptiveCruiseControl/assets/50420981/63ef3332-4b62-4942-abf0-f0fbd4e1e2d4)


### PID Control
-  Controling Motor based on PID(Proportional-Integral-Derivative)
  <br></br>
  ![pid제어](https://github.com/NahyunEE/AdaptiveCruiseControl/assets/50420981/610dd0cb-79c0-4f12-9977-ae13f6ef8425)

### ASPICE 
- All process is based on A-SPICE 3.1
<br></br>
![프로젝트일정](https://github.com/NahyunEE/AdaptiveCruiseControl/assets/50420981/4b8ef373-9e1a-4140-9c10-29f9030b4ee5)

## Comment

- uart.c and uart.h is for UART to get Motor State to PC with TeraTerm



