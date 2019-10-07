const d2Pin = 4;
const low = 0;
const high = 1;

function digitalWrite(pin, value) {
  return {
    "function": "dw",
    "params": {
      "pin": pin,
      "value": value
    }
  };
}

function delay(ms) {
  return {
    "function": "d",
    "params": {
      "ms": ms
    }
  };
}

exports.handler = async (event) => {
  const queryParams = event["query"];
  
  console.log(queryParams);
  if(queryParams['owner'] == "casabatata") {
    const response = {
        "instructions":[
          digitalWrite(d2Pin, high),
          delay(10000),
          digitalWrite(d2Pin, low)
        ]
    };
    console.log(JSON.stringify(response));
    return response;
  }
};

