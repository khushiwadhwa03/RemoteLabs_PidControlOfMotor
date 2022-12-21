// timer

{/* // Set the date we're counting down to */ }
var countDownDate = new Date().getTime();
if(localStorage.getItem("countDownDate") === null)
        localStorage.setItem("countDownDate",countDownDate);

console.log(countDownDate);

// Update the count down every 1 second
var x = setInterval(function () {

    // Get today's date and time
    var now = new Date().getTime();

    // Find the distance between now and the count down date
    countDownDate = localStorage.getItem("countDownDate");
    var distance = countDownDate + 900000 - now;

    // Time calculations for days, hours, minutes and seconds
    var minutes = Math.floor((distance % (1000 * 60 * 60)) / (1000 * 60));
    var seconds = Math.floor((distance % (1000 * 60)) / 1000);

    // Display the result in the element with id="demo"
    document.getElementById("demo").innerHTML = minutes + "m&nbsp;&nbsp; " + seconds + "s ";

    // If the count down is finished, write some text
    if (distance < 0) {
        clearInterval(x);
        document.getElementById("demo").innerHTML = "EXPIRED";
    }
}, 1000);
