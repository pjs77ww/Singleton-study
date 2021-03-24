# 싱글톤패턴(Singleton Pattern)

### 정의

생성자가 여러 차례 호출되더라도 실제로 생성되는 객체는 하나이고, 최초 생성 이후에 호출된 생성자는 최초의 생성자가 생성한 객체를 리턴한다. 주로 공통된 객체를 여러개 생성해서 사용하는 DBCP(DataBase Connection Pool)와 같은 상황에서 많이 사용된다.

![](./Singleton_UML_class_diagram.svg.png)

#### 싱글턴을 왜 사용하는가?

1. **한 번도 사용하지 않는다면 아예 인스턴스를 생성하지 않는다.**
2. **런타임에 초기화된다.**
   - 보통 싱글턴 대안으로 정적 맴버 변수를 많이 사용한다. But 정적 맴버 변수는 자동으로 초기화 되는 문제가 있다. 즉, 컴파일러는 main 함수를 호출하기 전에 정적 변수를 초기화하기 때문에 프로그램이 실행된 다음에야 알 수 있는 정보를 활용할 수 없다. 또한 정적 변수 초기화 순서도 보장되지 않기 때문에 한 정적 변수가 다른 정적 변수에 안전하게 의존할 수도 없다.
3. **싱글턴을 상속할 수 있다.**
   - 파일 시스템 래퍼가 크로스 플랫폼을 지원해야 한다면 추상 인터페이스를 만든 뒤, 플랫폼마다 구체 클래스를 만들면 된다.



#### 싱글턴의 문제점

1. **전역 변수의 이해복잡성**
   - 전역 변수는 코드를 이해하기 어렵게 만든다. 전역 변수로 선언된 클래스가 있다면 전역에 선언된 데이터를 전부 확인해야 이해할 수 있다.
2. **전역 변수는 커플링을 조장한다**
   - 전역에 변화를 조금만 줘도 전체적인 아키텍처에 문제를 일으킬 수 있다.
   - 인스턴스에 대한 접근을 통제함으로써 커플링을 통제할 수 있다.
3. **전역 변수는 멀티스레딩 같은 동시성 프로그래밍에 알맞지 않다**
   - 다른 스래드가 전역 데이터에 무슨 작업을 하는지 모를 때 교착상태, 경쟁상태 등 찾기 어려운 스레드 동기화 버그가 생기기 쉽다.
4. **게으른 초기화는 제어할 수 없다**
   - 시스템을 초기화할 때 메모리 할당, 리소스 로딩 등 할 일이 많다 보니 시간이 꽤 걸릴 수 있다.



### 예시

파일 시스템 API와 같이 외부 시스템과 상호작용하면서 전역 상태를 관리해야 하기 때문에 인스턴스가 여러 개면 제대로 작동하지 않는다.

파일 시스템 클래스로 들어온 호출은 이전 작업 전체에 대해서 접근할 수 있어야 한다. 아무 데서나 파일 시스템 클래스 인스턴스를 만들 수 있다면 다른 인스턴스에서 어떤 작업을 진행중인지 알 수 없다. 이런 경우 싱글턴으로 만들면 클래스가 인스턴스를 하나만 가지도록 강제할 수 있다.



### 기본구조

```C++
class FileSystem { 
public: 
    static FileSystem& instance() { 
        // 게으른 초기화
        // instance가 비어있는지 확인해주고 비어있다면 생성한다.
        // 생성자가 private에 있고 static으로 선언되기 때문에 단 한번밖에 접근이 안된다.
        if(instance_ == nullptr) { 
            instance_ = new FileSystem(); 
        }
        
        return *instance_; 
    }
        
private:
    // 생성자를 private에 둠으로써 한번 밖에 접근하지 못하게 한다.
    FileSystem() {}
    static FileSystem* instance_;
};
```

- instance가 비어있는지(NULL) 확인해주고 비어있다면 생성한다.
- 생성자가 private에 있고 static으로 선언되기 때문에 단 한번밖에 접근이 안된다.



### 싱글턴 활용

1. **상위 글래스 생성**

   ```C++
   class FileSystem { 
   public: 
       virtual ~FileSystem() {}
       virtual char* readFile(char* path) = 0;
       virtual void writeFile(char* path, char* contents) = 0; 
   };
   ```

2. **플랫폼 별 하위 클래스 정의**

   ```C++
   class PS3FileSystem: public FileSystem {
   public:
       virtaul char* readFile(char* path) {
           // 소니의 파일 IO API를 사용한다... 
       }
       virtaul void writeFile(char* path, char* contents) { 
           // 소니의 파일 IO API를 사용한다... 
       } 
   };
   
   class WIIFileSystem: public FileSystem{ 
   public: 
       virtaul char* readFile(char* path) {
           // 닌텐도의 파일 IO API를 사용한다... 
       }
       virtaul void writeFile(char* path, char* contents) { 
           // 닌텐도의 파일 IO API를 사용한다... 
       } 
   };
   
   ```

3. **전체 시스템 클래스를 싱글턴으로 만든다.**

   ```C++
   class FileSystem {
   public:
       static FIleSystem& instance();
       
       virtual ~FileSystem() {}
       virtual char* readFile(char* path) = 0;
       virtual void writeFile(char* path, char* contents) = 0;
       
   protected:
       FileSystem() {} 
   };
   ```

4. **인스턴스를 생성한다.**

   ```C++
   FileSystem& FileSystem::instance() {
   #if PLATFROM == PLAYSTATION3
       static FileSystem* instance = new PS3FileSystem();
   #elif PLATFORM == WII
       static FileSystem* instance = new WiiFileSystem();
   #endif
       return *instance;
   }
   ```

   



### 참고개념

#### Static(정적변수)

- 정적으로 할당되는 변수이며, 프로그램 실행 전반에 걸쳐 변수의 수명이 유지된다.



## 참고자료

- 위키백과_싱글턴 패턴 (https://ko.wikipedia.org/wiki/%EC%8B%B1%EA%B8%80%ED%84%B4_%ED%8C%A8%ED%84%B4)
- 위키백과 정적변수(https://ko.wikipedia.org/wiki/%EC%A0%95%EC%A0%81_%EB%B3%80%EC%88%98)

- 소년코딩_C++ 디자인 패턴 05. 싱글턴 패턴, Singleton Pattern (https://boycoding.tistory.com/109)

