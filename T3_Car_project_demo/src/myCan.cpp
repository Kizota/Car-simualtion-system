#include "myCan.h"
#define MAX_SENSOR_VALUE 25

int Id_mask_create(Id_guard *id_guard, unsigned long mask, unsigned long *filters, uint8_t len)
{
    if (id_guard == nullptr || filters == nullptr)
    {
        return 0;
    }
    id_guard->mask = mask;
    id_guard->filter = filters;
    id_guard->len = len;

    return 1;
}

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
int CAN_module_create(CAN_module *can, MCP_CAN *CAN, uint8_t speed, unsigned long tx_id, unsigned long *rx_mask, uint8_t nofmasks, unsigned long *rx_filter, uint8_t nofilters)
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

    // init CAN masks
    for (int no = 0; no < can->masks_len; no++)
    {
        Serial.println(*(can->rx_mask + no), HEX);
        can->can->init_Mask(no, 0, *(can->rx_mask + no));
    }

    // init CAN filters
    for (int no = 0; no < can->filters_len; no++)
    {
        Serial.println(*(can->rx_filter + no), HEX);
        can->can->init_Filt(no, 0, *(can->rx_filter + no));
    }

    // set CAN mode
    can->can->setMode(MCP_NORMAL);

    Serial.println("CAN Bus init ok!\n");
}

int CAN_send_message(CAN_module *can, Command_t *command_list, int no)
{
    if (can == nullptr || command_list == nullptr)
    {
        return 0;
    }

    unsigned long id = can->tx_id + (command_list + no)->comp_id;

    can->can->sendMsgBuf(id, (uint8_t)MAX_BUFFER, &(command_list + no)->command);

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
        Serial.print("send messge: ");
        Serial.println(no);
        CAN_send_message(can, command_list, no);
        timer->pre_time = *timer->now;
    }

    return 1;
}

// sensor node
int Handle_sending_sensor_data(CAN_module *can, Command_t *command_list, int cmd_len, Timer *timer)
{
    if (can == nullptr || command_list == nullptr || timer == nullptr)
    {
        return 0;
    }

    if (*timer->now - timer->pre_time > timer->interval)
    {
        srand(time(NULL));
        uint8_t no = rand() % cmd_len;

        (command_list + no)->command = (uint8_t)(rand() % MAX_SENSOR_VALUE);

        CAN_send_message(can, command_list, no);
        timer->pre_time = *timer->now;
    }

    return 1;
}

// message filter

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
    }

    return 1;
}

int CAN_check_message(Id_guard *id_guard, Message *msg)
{
    if (id_guard == nullptr || msg == nullptr)
    {
        return 0;
    }

    int is_valid = 0;

    unsigned long concerned_bits = msg->tx_id & id_guard->mask;

    // check concern bit with filter
    for (int i = 0; i < id_guard->len && !is_valid; i++)
    {
        if (*(id_guard->filter + i) == concerned_bits)
        {
            is_valid = 1;
        }
    }

    return is_valid;
}

// print the CAN message
int CAN_print_message(Message *msg)
{
    if (msg == nullptr)
    {
        return 0;
    }

    if (msg->content == msg->pre_content)
    {
        return 0;
    }
    Serial.println("- commands come in - ");

    Serial.print("id: ");
    Serial.println(msg->tx_id, HEX);
    Serial.print("len: ");
    Serial.println(msg->len);
    Serial.print("cotent: ");
    Serial.println(msg->content);

    msg->pre_content = msg->content;

    return 1;
}