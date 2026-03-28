#include"rclcpp/rclcpp.hpp"
#include"cashier_system/msg/bill.hpp"
#include"cashier_system/srv/get_status.hpp"
#include<map>

class InventoryNode: public rclcpp::Node{
    public:
    InventoryNode(): Node("inventory_node"), total_income_(0.0){
        subscription_=this->create_subscription<cashier_system::msg::Bill>(
            "bill", 10,
            std::bind(&InventoryNode::bill_callback, this, std::placeholders::_1)

);
            service_ = this->create_service<cashier_system::srv::GetStatus>(
    "get_status",
    std::bind(&InventoryNode::handle_status, this,
              std::placeholders::_1, std::placeholders::_2)
);
        
    }

    private:
    std::map<std::string, int> inventory_;
    float total_income_;
    

    //whenever something is published on /bill, run my function
    rclcpp:: Subscription<cashier_system::msg::Bill> ::SharedPtr subscription_;
    rclcpp::Service<cashier_system::srv::GetStatus>::SharedPtr service_;

    
    //runs automatically when message arrives
    void bill_callback(const cashier_system::msg::Bill::SharedPtr msg){


    //we did not initialize the stock yet, so it goes negative    
    inventory_[msg->item_name]-=msg->quantity;

    //income calculation
    total_income_+=msg->quantity*msg->price;

    RCLCPP_INFO(this->get_logger(),
        "Received: %s x%d | Total Income: %.2f",

        msg->item_name.c_str(),
        msg->quantity,
        total_income_
    );
}


    

    void handle_status(
        const std::shared_ptr<cashier_system::srv::GetStatus::Request>,
        std::shared_ptr<cashier_system::srv::GetStatus::Response> response
    ){
        RCLCPP_INFO(this->get_logger(), "Service called!");
        std::string result= "Inventory: \n";
        for(auto &item : inventory_){
            result+=item.first + ": "+ std::to_string(item.second)+ "\n";
        }

        result+= "Total Income: "+ std::to_string(total_income_);
        response->status=result;
    }
};

int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<InventoryNode>());
    rclcpp::shutdown();

    return 0;
}


