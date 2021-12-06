/** 
 * 假定变量invoice是解析invoices.json得到的数组中下标为0的Json::Value变量
 * 假定变量plays是解析plays.json得到的Json::Value变量
 */

// Json::Value变量是jsoncpp中定义的类，详见https://github.com/open-source-parsers/jsoncpp

std::string printBillInfo(const Json::Value& invoice, const Json::Value& plays) {
    int totalAmount = 0;
    int volumeCredits = 0;

    std::string result;
    result = std::string("Statement for ") +
             invoice["customer"].asString() +
             std::string("\n");

    Json::Value performances = invoice["performances"];
    int size = performances.size();
    for (int i = 0; i < size; ++i) {
        Json::Value perf = performances[i];
        const Json::Value play = plays[perf["playID"].asString()];
        int thisAmount = 0;

        // C++中switch不支持传入string，故此处改为if-else
        std::string playType = play["type"].asString();
        int audienceCnt = perf["audience"].asInt();
        if (playType == "tragedy") {
            thisAmount = 40000;
            if (audienceCnt > 30) {
                thisAmount += 1000 * (audienceCnt - 30);
            }
        } else if (playType == "comedy") {
            thisAmount = 30000;
            if (audienceCnt > 20) {
                thisAmount += 10000 + 500 * (audienceCnt - 30);
            }
        } else {
            return "error, unknown type";
        }

        // add volume credits
        volumeCredits += max(audienceCnt - 30, 0);
        // add extra credit for every ten comedy attendees
        if ("comedy" == playType) {
            volumeCredits += floor(audienceCnt / 5);
        }

        // print line for this order
        result += play["name"].asString() + " " +
                  to_string(thisAmount / 100) +
                  "(" + to_string(audienceCnt) + "seats)\n";
        totalAmount += thisAmount;
    }

    result += "Amount owed is " + to_string(totalAmount / 100) + "\n";
    result += "You earned " + to_string(volumeCredits) + "credits\n";
    return result;
}

// 问题:
/**
 * 1、代码组织不清晰，一个函数干了多件事情，导致函数过长；
 * 2、不利于扩展；
 */
