if (ENVIRONMENT_IS_NODE) {
  console.log('Mounting /working as NODEFS at current directory.');
  FS.mkdir('/working');
  FS.mount(NODEFS, {root: '.'}, '/working');
}
