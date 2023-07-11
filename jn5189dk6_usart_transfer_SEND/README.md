## JN5180dk6_usart_transfer_SEND

학습 목적으로 NXP SDK examples 코드를 참고해, 간단하게 USART통신을 활용하는 프로젝트 작성. 

Requirement: 

- 사용자로부터 콘솔을 통해 문자하나를 입력 받음 ([link]([Intern-2023-nxp/jn5189dk6_usart_transfer_SEND/source/usart_transfer_send.c at main · merlot-platform/Intern-2023-nxp · GitHub](https://github.com/merlot-platform/Intern-2023-nxp/blob/main/jn5189dk6_usart_transfer_SEND/source/usart_transfer_send.c))). 
- PIO11 포트를 USART TX로 설정하여 입력된 문자를 송신 ([link]([Intern-2023-nxp/jn5189dk6_usart_transfer_SEND/board/pin_mux.c at main · merlot-platform/Intern-2023-nxp · GitHub](https://github.com/merlot-platform/Intern-2023-nxp/blob/main/jn5189dk6_usart_transfer_SEND/board/pin_mux.c))).