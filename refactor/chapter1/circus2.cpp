/** 
 * 假定变量invoice是解析invoices.json得到的数组中下标为0的Json::Value变量
 * 假定变量plays是解析plays.json得到的Json::Value变量
 * 
 * 对circus1.cpp代码进行优化：
 * 1、将不同的功能提炼成对应的函数，一个函数干一件事
 * 2、修改名称，提升函数的表达能力
 * 3、动态类型语言可以将类型加入到名称中，用于追踪变量的类型
 * 4、尽量移除局部变量
 * 
 * 
 * 注：对于重构过程的性能问题，大多数情况下可以忽略它。如果重构引入了性能损耗，先完成重构，再做性能优化。
 */

int amountFor(const Json::Value& perf, const Json::Value& play) {
    int result = 0;
    std::string playType = play["type"].asString();
    int audienceCnt = perf["audience"].asInt();
    if (playType == "tragedy") {
        result = 40000;
        if (audienceCnt > 30) {
            result += 1000 * (audienceCnt - 30);
        }
    } else if (playType == "comedy") {
        result = 30000;
        if (audienceCnt > 20) {
            result += 10000 + 500 * (audienceCnt - 30);
        }
    } else {
        return -1;
    }

    return result;
}

Json::Value playFor(const Json::Value& perf, const Json::Value& plays) {
    return plays[perf["playID"].asString()];
}

int totalAmount(const Json::Value &invoice, const Json::Value &plays) {
    int result = 0;
    Json::Value performances = invoice["performances"];
    int size = performances.size();
    for (int i = 0; i < size; ++i) {
        Json::Value perf = performances[i];
        const Json::Value play = playFor(perf, plays);

        result += amountFor(perf, play);
    }

    return result;
}

int volumCreditsFor(const Json::Value &perf, const Json::Value& play) {
    int result = 0;
    std::string playType = play["type"].asString();
    int audienceCnt = perf["audience"].asInt();

    // add volume credits
    result += max(audienceCnt - 30, 0);
    // add extra credit for every ten comedy attendees
    if ("comedy" == playType) {
        result += floor(audienceCnt / 5);
    }

    return result;
}

int totalVolumeCredits(const Json::Value &invoice, const Json::Value &plays) {
    int result = 0;
    Json::Value performances = invoice["performances"];
    int size = performances.size();
    for (int i = 0; i < size; ++i) {
        Json::Value perf = performances[i];
        const Json::Value play = playFor(perf, plays);

        result += volumCreditsFor(perf, play);
    }

    return result;
}

std::string printBillInfo(const Json::Value& invoice, const Json::Value& plays) {
    std::string result = "Statement for " +
                         invoice["customer"].asString() +
                         "\n";

    Json::Value performances = invoice["performances"];
    int size = performances.size();
    for (int i = 0; i < size; ++i) {
        Json::Value perf = performances[i];
        const Json::Value play = playFor(perf, plays);
        std::string playType = play["type"].asString();
        int audienceCnt = perf["audience"].asInt();

        // print line for this order
        result += play["name"].asString() + " " +
                  to_string(amountFor(perf, play) / 100) +
                  "(" + to_string(audienceCnt) + "seats)\n";
    }

    result += "Amount owed is " + to_string(totalAmount() / 100) + "\n";
    result += "You earned " + to_string(totalVolumeCredits()) + "credits\n";
    return result;
}
