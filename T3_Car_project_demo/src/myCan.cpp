#include "myCan.h"

int Timer_create(Timer *timer, unsigned long *now, unsigned long interval)
{
    if (timer == nullptr || now == nullptr)
    {
        return 0;
    }

    timer->now = now;
    timer->interval = interval;
    timer->pre_time = 0;

    return 1;
}

/* ----  Transmitter   ---- */
// CAN function definition
int CAN_module_create(CAN_module *can, MCP_CAN *CAN, uint8_t speed, uint8_t tx_id, uint8_t *rx_mask, uint8_t nofmasks, uint8_t *rx_filter, uint8_t nofilters)
{
    if (can == nullptr || CAN == nullptr || rx_mask == nullptr || rx_filter == nullptr)
    {
        return 0;
    }

    can->can = CAN;
    can->speed = speed;

    can->tx_id = tx_id;

    can->rx_mask = rx_mask;
    can->rx_filter = rx_filter;

    can->masks_len = nofmasks;
    can->filters_len = nofilters;
    return 1;
}

void CAN_setup(CAN_module *can)
{
    // connect to CAN network
    while (CAN_OK != can->can->begin(MCP_STDEXT, can->speed, MCP_16MHZ))
    {
        Serial.println("CAN BUS init failed");
        delay(100);
    }
    

    // // init CAN masks
    // for (int no = 0; no < can->masks_len; no++)
    // {
    //     Serial.println(*(can->rx_mask + no), HEX);
    //     can->can->init_Mask(no, 0, *(can->rx_mask + no));
    // }

    // // init CAN filters
    // for (int no = 0; no < can->filters_len; no++)
    // {
    //     Serial.println(*(can->rx_filter + no), HEX);
    //     can->can->init_Filt(no, 0, *(can->rx_filter + no));
    // }

    // set CAN mode
    can->can->setMode(MCP_NORMAL);

    Serial.println("CAN Bus init ok!\n");
}

int CAN_send_message(CAN_module *can, Command_t *command_list, Message_t msg)
{
    if (can == nullptr || command_list == nullptr)
    {
        return 0;
    }

    unsigned long id = can->tx_id + command_list->comp_id;

    can->can->sendMsgBuf(id, (uint8_t)MAX_BUFFER, &(command_list + msg)->command);

    return 1;
}

// steering wheels - node
int Handle_sending_random_signal_comand(CAN_module *can, Command_t *command_list, int cmd_len, Timer *timer)
{
    if (can == nullptr || command_list == nullptr || timer == nullptr)
    {
        return 0;
    }

    if (*timer->now - timer->pre_time > timer->interval)
    {
        srand(time(NULL));
        uint8_t no = rand() % cmd_len;
        Serial.println("send messge!\n");
        CAN_send_message(can, command_list, (Message_t)no);
        timer->pre_time = *timer->now;
    }

    return 1;
}

//  Read the incoming message from the CAN network
int CAN_read_message(CAN_module *can, Message *msg)
{
    if (msg == nullptr || can == nullptr)
    {
        return 0;
    }

    if (CAN_MSGAVAIL == can->can->checkReceive())
    {
        can->can->readMsgBuf(&msg->tx_id, &msg->len, &msg->content);
        CAN_print_message(msg);
    }

    return 1;
}

// print the CAN message
int CAN_print_message(Message *msg)
{
    if (msg == nullptr)
    {
        return 0;
    }

    Serial.print("id: ");
    Serial.println(msg->tx_id, HEX);
    Serial.print("len: ");
    Serial.println(msg->len);
    Serial.print("cotent: ");
    Serial.println(msg->content);

    return 1;
}