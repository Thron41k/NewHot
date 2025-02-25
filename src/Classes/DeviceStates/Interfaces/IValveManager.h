class IValveManager {
    public:
      virtual short GetPercent() const = 0;
      virtual void SetPercent(short percent) = 0;
      virtual ~IValveManager() = default;
    };