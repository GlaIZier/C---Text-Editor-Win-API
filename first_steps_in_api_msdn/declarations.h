#pragma once

// Declare functions
// моя обработка сообщений для акселераторов
void ProcessMessage(MSG& mymsg); 
// для регистрации класса
bool Reg_Class(HINSTANCE hInstance);    // возвращает 0 в хорошем случае
// создание визуальных компонентов
bool Create_Components(HINSTANCE hInstance,int nCmdShow);  // возвращает 0 в хорошем случае



