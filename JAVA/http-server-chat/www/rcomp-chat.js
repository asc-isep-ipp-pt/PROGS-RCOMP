
// global variables
var nextMsg;
var mArea,nicknameBox, messageBox, hints; // defined only after the document is loaded

function loadAndStart() {
    mArea=document.getElementById("messages");
    nicknameBox=document.getElementById("nickname");
    messageBox=document.getElementById("message");
    hints=document.getElementById("hints");
    nextMsg=0;
    setTimeout(getNextMessage, 1000);
    }

function getNextMessage() {
    var request = new XMLHttpRequest();
    
    request.onload = function() {
        if(nextMsg===0) { 
		mArea.value = "";
        	mArea.style.color="blue";
	}
        mArea.value = mArea.value + this.responseText + "\r\n";
        mArea.scrollTop = mArea.scrollHeight; // scroll the textarea to make last lines visible
        nextMsg=nextMsg+1; 
        setTimeout(getNextMessage, 100);
        };

    request.onerror = function() { 
        nextMsg=0;
        mArea.value = "Server not responding.";
        mArea.style.color="red";
        setTimeout(getNextMessage, 1000); 
    };

    request.ontimeout = function() { 
        nextMsg=0;
        mArea.value = "Server not responding.";
        mArea.style.color="red";
        setTimeout(getNextMessage, 100); 
    };
        
        
    request.open("GET", "/messages/" + nextMsg, true);
    if(nextMsg===0) request.timeout = 1000;
    // Message 0 is a server's greeting, it should always exist
    // no timeout, for following messages, the server responds only when the requested
    // message number exists
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
        return;
        }
    var POSTrequest = new XMLHttpRequest();
    nicknameBox.disabled=true;
    POSTrequest.open("POST", "/messages", true);
    POSTrequest.timeout = 5000;
    POSTrequest.send("(" + nicknameBox.value + ") " + messageBox.value);
    }
	


