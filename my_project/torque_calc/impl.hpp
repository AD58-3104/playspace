#include <cmath>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <fstream>
#include <optional>
#include <vector>
#include <memory>
struct link{
    std::weak_ptr<link> parent_;
    using child_link = link;
    std::weak_ptr<child_link> roll_;
    std::weak_ptr<child_link> pitch_;
    std::weak_ptr<child_link> yaw_;
    double mass_;
    std::vector<double> inertia_;
    link(std::shared_ptr<link> parent,const double& mass,const std::vector<double>& inertia):parent_(parent),roll_(),pitch_(),yaw_(),mass_(mass),inertia_(inertia) {
        assert(inertia.size() == 3); //"inertia must have three item"
    }
};

class robot{
    private:
    std::vector<std::shared_ptr<link>> link_list_;
};

