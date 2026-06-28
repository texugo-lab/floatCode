
// Dark mode //

let darmode = localStorage.getItem('darkmode');
const themeSwitch = document.getElementById('theme-switch');

function enableDarkmode() {
	document.body.classList.add('darkmode');
	localStorage.setItem('darkmode', 'active');
}

function disableDarkmode() {
	document.body.classList.remove('darkmode');
	localStorage.setItem('darkmode', null);
}

if(darkmode === 'active')
	document.body.classList.add('darkmode');

function toggleMode(){
	darkmode = localStorage.getItem('darkmode');
	if(darkmode.match("active"))
		disableDarkmode();
	else
		enableDarkmode();
}

// End //


function openSite(site) {
	window.open(site, "_self");
}

function toggleSidebar(option){
	const sidebar = document.querySelector('.sidebar');
	sidebar.style.right = option;
}
