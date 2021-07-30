#include <memory>
#include <vector>
#include

namespace Citbrains
{

    struct OtherRobotInfomation
    {
    };

    class InfoShare
    {
    public:
        [[nodiscard]] getter(const int id )const noexcept; 

    private:
        std::vector<std::unique_ptr<Citbrains::otherRobotInfomation>> robot_data_list_;
        MessageProto::SharingData sharing_data;
    };

}