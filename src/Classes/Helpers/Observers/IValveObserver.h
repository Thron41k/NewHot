class IValveObserver {
    public:
      virtual void OnValvePercentChanged(short percent) = 0;
      virtual ~IValveObserver() = default;
    };