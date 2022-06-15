import './style.css';
const urlBase= import.meta.env.PROD ? '' :  'http://leddimmer.fritz.box';

window.setCurrent = function (value: string) {
  const headers = {'Content-Type': 'text/plain'};
  fetch(
    `${urlBase}/current`, {
    headers,
    method: 'POST',
    body: value
  });
}

async function getAndWriteCurrent() {
  const value = await (await fetch(`${urlBase}/current`)).text();
  const input = document.querySelector<HTMLInputElement>('#manualInput');

  input!.value = value;
  (<HTMLOutputElement>input!.nextElementSibling).value = value;
}

window.getAndWriteLog = async function () {
  const value = await (await fetch(`${urlBase}/log`)).text();
  document.querySelector<HTMLTextAreaElement>('#log')!.textContent = value;
}

async function getAndWriteTime() {
  const time = await (await fetch(`${urlBase}/time`)).text();
  (<HTMLOutputElement>document.querySelector('#currentTime')).textContent = time;
}

window.forceTimeUpdate = async function () {
  await fetch(`${urlBase}/time/update`);
  getAndWriteTime();
}

async function getAndWriteConfig(index: number) {
  const config = await (await fetch(`${urlBase}/config/${index}`)).json();
  const parent = document.querySelector(`[data-config-index="${index}"]`);
  if(parent === null) {return;}
  parent.querySelector<HTMLInputElement>(`[name=enabled]`)!.checked = config.enabled;
  parent.querySelector<HTMLInputElement>(`[name=startTime]`)!.value = config.startTime;
  parent.querySelector<HTMLInputElement>(`[name=endTime]`)!.value = config.endTime;
  parent.querySelector<HTMLInputElement>(`[name=percentage]`)!.value = config.percentage;
  parent.querySelector<HTMLDivElement>(`.loading`)!.style.display = 'none';
  parent.querySelectorAll<HTMLInputElement>(`input[name="daysOfWeek"]`).forEach((elem) =>
    elem.checked = config.daysOfWeek.includes(elem.value));
  parent.querySelectorAll<HTMLInputElement>(`[type="time"]`).forEach(printEffective);
}

window.updateConfig = async function(index: number) {
  document
    .querySelector<HTMLDivElement>(`[data-config-index="${index}"] .loading`)!.style
    .display = 'block';
  const form = document.querySelector<HTMLFormElement>(`[data-config-index="${index}"] form`);
  if(form == null) {return;}
  const data = new URLSearchParams(new FormData(form) as any);
  data.set('daysOfWeek', data.getAll('daysOfWeek').join(','));
  await fetch(
    `${urlBase}/config/${index}`, {
    method: 'POST',
    body: data
  });
  await getAndWriteConfig(index);
}

window.setCurrentManually =function () {
  setCurrent(document.querySelector<HTMLInputElement>('#manualInput')?.value ?? '');
}

window.printEffective = function (elem: HTMLInputElement) {
  const offsetInMinutes = new Date().getTimezoneOffset();
  const utcHours = elem.valueAsDate?.getUTCHours() ?? 0;
  const utcMinutes = elem.valueAsDate?.getUTCMinutes() ?? 0;

  const localHours = parseInt((utcHours * 60 + utcMinutes - offsetInMinutes) / 60 % 24 as any);
  const localMinutes = (utcHours * 60 + utcMinutes - offsetInMinutes) % 60;
  const localString = localHours.toString().padStart(2,'0') + ":"
      + localMinutes.toString().padStart(2,'0');
  elem.nextElementSibling!.textContent = localString;
}

getAndWriteTime();
setInterval(getAndWriteTime, 10000);
getAndWriteLog();
setInterval(getAndWriteLog, 10000);
getAndWriteConfig(0);
getAndWriteConfig(1);
getAndWriteConfig(2);
getAndWriteConfig(3);
getAndWriteConfig(4);
getAndWriteCurrent();

if(location.search.includes('mode=dark')) {
  document.querySelector<HTMLElement>('html')!.dataset.mode='dark';
}