# task-management
`course work |FCSaN|`



## Cистема управления задачами будет включать в себя :

- Cистема ролей (user, task manager, admin)

- Cистема приоритетности выполняемых задач

- Система доступа(admin -> task_manager(1...) -> user(1...))

- Хранение данных с помощью sqllite3 
их обработка(Добавление новых задач,редактирование,удаление)

- Визуальная состовляющаю(QTcreator)

- Cистему авторизации/регистриации с использованием протокола smtp

## Структура проекта :

###  *src /* 

    main.cpp
        user.h
             user_manager.h 
                email_sender.h 
                     sending_email.cpp 
                         sending_email.h
 ### *include /* 
    
     sqlite 

### *lab /*
 
    CMakeLists.txt 
    LICENSE
    .gitignore 
    README.md




# [SonarCloud]( https://sonarcloud.io/project/overview?id=splitmindq_task-management )
[![Quality gate](https://sonarcloud.io/api/project_badges/quality_gate?project=splitmindq_task-management)](https://sonarcloud.io/summary/new_code?id=splitmindq_task-management)

