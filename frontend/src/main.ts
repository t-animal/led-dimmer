import './style.css';

// const urlBase='';
const urlBase='http://leddimmer.fritz.box';

function setCurrent(value) {
  const headers = {'Content-Type': 'text/plain'};
  fetch(
    `${urlBase}/current`, {
    headers,
    method: 'POST',
    body: value
  });
}

async function getAndWriteTime() {
  const time = await (await fetch(`${urlBase}/time`)).text();
  document.querySelector('#currentTime').textContent = time;
}

function forceTimeUpdate() {
  const time = fetch(`${urlBase}/time/update`);
  getAndWriteTime();
}

async function getAndWriteConfig(index) {
  const config = await (await fetch(`${urlBase}/config/${index}`)).json();
  document.querySelector(`[data-config-index="${index}"] [name=enabled]`).checked = config.enabled;
  document.querySelector(`[data-config-index="${index}"] [name=startTime]`).value = config.startTime;
  document.querySelector(`[data-config-index="${index}"] [name=endTime]`).value = config.endTime;
  document.querySelector(`[data-config-index="${index}"] [name=percentage]`).value = config.percentage;
  document.querySelector(`[data-config-index="${index}"] .loading`).style.display = 'none';
  document.querySelectorAll(`[data-config-index="${index}"] input[name="daysOfWeek"]`).forEach((elem) =>
    elem.checked = config.daysOfWeek.includes(elem.value));
  document.querySelectorAll(`[data-config-index="${index}"] [type="time"]`).forEach(printEffective);
}

async function updateConfig(index) {
  document.querySelector(`[data-config-index="${index}"] .loading`).style.display = 'block';
  const form = document.querySelector(`[data-config-index="${index}"] form`);
  const data = new URLSearchParams(new FormData(form));
  data.set('daysOfWeek', data.getAll('daysOfWeek').join(','));
  await fetch(
    `${urlBase}/config/${index}`, {
    method: 'POST',
    body: data
  });
  await getAndWriteConfig(index);
}

function setCurrentManually() {
  setCurrent(document.querySelector('#manualInput').value);
}

function printEffective(elem) {
  const offsetInMinutes = new Date().getTimezoneOffset();
  const utcHours = elem.valueAsDate.getUTCHours();
  const utcMinutes = elem.valueAsDate.getUTCMinutes();

  const localHours = parseInt((utcHours * 60 + utcMinutes - offsetInMinutes) / 60 % 24);
  const localMinutes = (utcHours * 60 + utcMinutes - offsetInMinutes) % 60;
  const localString = localHours.toString().padStart(2,'0') + ":"
      + localMinutes.toString().padStart(2,'0');
  elem.nextElementSibling.textContent = localString;
}

getAndWriteTime();
setInterval(getAndWriteTime, 10000);
getAndWriteConfig(0);
getAndWriteConfig(1);
getAndWriteConfig(2);
getAndWriteConfig(3);
getAndWriteConfig(4);

if(location.search.includes('mode=dark')) {
  document.querySelector('html').dataset.mode='dark';
}