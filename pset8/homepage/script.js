// Function to make image do a barrel roll
    function sleep(ms) {
        return new Promise(resolve => setTimeout(resolve, ms));
    }

    async function rotate(id) {
        let counter = 0;
        button = document.getElementById(id);
        setTimeout(function(){button.disabled = false;},1);
        if (button.disabled == false) {
            for (let i = 0; i < 360 ; i++) {
                counter++;
                button.setAttribute('style','transform: rotate(' + counter + 'deg)');
                await sleep(1);
                button.disabled = true;
            }
        }
    }

    async function backrotate(id) {
        let counter = 0;
        button = document.getElementById(id);
        setTimeout(function(){button.disabled = false;},1);
        if (button.disabled == false) {
            for (let i = 0; i < 360 ; i++) {
                counter--;
                button.setAttribute('style','transform: rotate(' + counter + 'deg)');
                await sleep(1);
                button.disabled = true;
            }
        }
    }
// Function to make image do a barrel roll


//important to ignore the first few white spaces from the string

function iam() {
    let description = document.querySelector('#submitname').value;
    let grammar = '&nbsp';
    let vowels = ['a', 'e', 'i', 'o', 'u'];

    // Set text to include a or an infront
    if (description.slice(0, 2) == 'a ') {
        document.querySelector('#nametyler').innerHTML = grammar + description.slice(2) + '.';
    }
    else if (description.slice(0, 3) == 'an ') {
        grammar = 'n ';
        document.querySelector('#nametyler').innerHTML = grammar + description.slice(3) + '.';
    }
    else if (description == "") {
        description = "bean";
        document.querySelector('#nametyler').innerHTML = grammar + description + '.';
    }
    else {
        for (let i = 0; i < vowels.length; i++) {
            if (description.charAt(0).toLowerCase() == vowels[i]) {
                grammar = 'n ';
                document.querySelector('#nametyler').innerHTML = grammar + description + '.';
                break;
            }
            else {
                grammar = '&nbsp';
                document.querySelector('#nametyler').innerHTML = grammar + description + '.';
            }
        }
    }
}