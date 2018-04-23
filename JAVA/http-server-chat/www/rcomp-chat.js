
// global variables
var nextMsg=0;
var mArea,nicknameBox, messageBox, hints; // defined only after the document is loaded

function loadAndStart() {
    mArea=document.getElementById("messages");
    nicknameBox=document.getElementById("nickname");
    messageBox=document.getElementById("message");
    hints=document.getElementById("hints");
    setTimeout(getNextMessage, 1000);
    }

function getNextMessage() {
    var request = new XMLHttpRequest();
    mArea.style.color="blue";
    if(nextMsg===0) mArea.value = "No messages yet ...";
    
    request.onload = function() {
        if(nextMsg===0) mArea.value = "";
        mArea.value = mArea.value + this.responseText + "\r\n";
        nextMsg=nextMsg+1; 
        mArea.scrollTop = mArea.scrollHeight; // scroll the textarea to make last lines visible
        setTimeout(getNextMessage, 100);
        };

    request.onerror = function() { 
        nextMsg=0;
        mArea.value = "Server not responding.";
        mArea.style.color="red";
        setTimeout(getNextMessage, 1000); 
    };
        
    request.open("GET", "/messages/" + nextMsg, true);
    // no timeout, the server responds only when the requested
    // message number is created
    request.send();
}

function postMessage() {
    hints.innerHTML="";
    if(nicknameBox.value === "") {
        hints.innerHTML="Settle a nickname before sending.";
        return;
        }
    if(messageBox.value === "") { 
        hints.innerHTML="Not sending empty message.";
        }
    var POSTrequest = new XMLHttpRequest();
    nicknameBox.disabled=true;
    POSTrequest.open("POST", "/messages", true);
    POSTrequest.send("(" + nicknameBox.value + ") " + messageBox.value);
    }
	


