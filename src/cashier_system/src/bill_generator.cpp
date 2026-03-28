#include "rclcpp/rclcpp.hpp"
#include "cashier_system/msg/bill.hpp"

class BillGenerator: public rclcpp::Node{//creation of the node
    public:

    //node constructor
    //name appears in logs and rqt_graph
    BillGenerator(): Node("bill_generator"){

        //creates a data sender, topic:bill, type bill, queue10 msgs
        publisher_= this-> create_publisher<cashier_system::msg::Bill>("bill",10);

        //ros nodes are event-driven, timier triggers publishing
        timer_=this->create_wall_timer(
            std::chrono::seconds(5),
            std::bind(&BillGenerator::publish_bill, this)
        );
    }

    private:

    //where input happens, msgs are created and sent
    void publish_bill(){

        //creates empty structure {item_name, quantity, price}
        auto msg= cashier_system::msg::Bill();

        std::string item;
        int quantity;
        float price;

        std::cout<<"\nEnter item: ";
        std::cin>> item;

        std::cout<<"Enter quantity: ";
        std::cin>>quantity;

        std::cout<<"Enter price: ";
        std::cin>>price;

        msg.item_name=item;
        msg.quantity=quantity;
        msg.price=price;

        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "Published: %s x %d", item.c_str(), quantity);
    }

    rclcpp::Publisher<cashier_system::msg::Bill>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);

    //keeps node alive, without this, the program exits immediately
    rclcpp::spin(std::make_shared<BillGenerator>());
    rclcpp::shutdown();

    return 0;
}