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
    // nullptr 초기화?
    static FileSystem* instance_;
};