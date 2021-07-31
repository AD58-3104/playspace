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
        void terminate(void);
        bool sendInfomationToOtherRobots();
        void changeColor(const int id, const int color);
		void setTimeFunc(float (*func)());
		float getTime(void);

    private:
        std::vector<std::unique_ptr<Citbrains::otherRobotInfomation>> robot_data_list_;
        MessageProto::SharingData sharing_data_;
        void receiveSharedInfomation();

        void receivingThreadLoop();
        void sendingThreadLoop();
    };

}